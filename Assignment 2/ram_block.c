
#include <linux/init.h>
#include <linux/blk_types.h>
#include <linux/vmalloc.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/errno.h>
#include <linux/bio.h>
#include <linux/blk-mq.h>
#include "ram_device.h"
#include "partition.h"

MODULE_DESCRIPTION("Assignment 2");
MODULE_AUTHOR("Aakanksha");
MODULE_LICENSE("GPL");


#define KERN_LOG_LEVEL		KERN_ALERT


#define RB_FIRST_MINOR		0
#define RB_MINOR_CNT		16


static struct my_block_dev {
	struct blk_mq_tag_set tag_set;
	struct request_queue *queue;
	struct gendisk *gd;
	size_t size;
} g_dev;

static int my_block_open(struct block_device *bdev, fmode_t mode)
{
	printk("Block driver opened");
	return 0;
}

static void my_block_release(struct gendisk *gd, fmode_t mode)
{
	printk("block driver closed");
}

static const struct block_device_operations my_block_ops = {
	.owner = THIS_MODULE,
	.open = my_block_open,
	.release = my_block_release
};



static void my_xfer_request(struct my_block_dev *dev, struct request *req)
{
	
	struct bio_vec bv;
	sector_t start_sector = blk_rq_pos(req);
	int dir = rq_data_dir(req);
	struct req_iterator iter;
	u8 *buffer;
	sector_t sector_offset;
	sector_offset = 0;
	unsigned int sectors;
	int ret = 0;
	unsigned int sector_cnt = blk_rq_sectors(req);
	
	rq_for_each_segment(bv, req, iter) {
		buffer = page_address(bv.bv_page) + bv.bv_offset;
		sectors = bv.bv_len / RB_SECTOR_SIZE;
		
		if (dir == 1)		 
			ramdevice_write(start_sector + sector_offset, buffer, sectors);
		else
			ramdevice_read(start_sector + sector_offset, buffer, sectors);
		sector_offset += sectors;
		}
	if (sector_offset != sector_cnt)
	{
		printk(KERN_ERR "rb: bio info doesn't match with the request info");
		ret = -EIO;
	}
}

static blk_status_t my_block_request(struct blk_mq_hw_ctx *hctx,
				     const struct blk_mq_queue_data *bd)
{
	struct request *rq;
	struct my_block_dev *dev = hctx->queue->queuedata;

	rq = bd->rq;
	blk_mq_start_request(rq);

	if (blk_rq_is_passthrough(rq)) {
		printk(KERN_NOTICE "Skip non-fs request\n");
		blk_mq_end_request(rq, BLK_STS_IOERR);
		goto out;
	}

	printk(KERN_LOG_LEVEL
		"request received: pos=%llu bytes=%u "
		"cur_bytes=%u dir=%c\n",
		(unsigned long long) blk_rq_pos(rq),
		blk_rq_bytes(rq), blk_rq_cur_bytes(rq),
		rq_data_dir(rq) ? 'W' : 'R');

	my_xfer_request(dev, rq);

	blk_mq_end_request(rq, BLK_STS_OK);

out:
	return BLK_STS_OK;
}

static struct blk_mq_ops my_queue_ops = {
	.queue_rq = my_block_request,
};
static u_int rb_major = 0;
static int create_block_device(struct my_block_dev *dev,int size)
{
	int err;

	dev->tag_set.ops = &my_queue_ops;
	dev->tag_set.nr_hw_queues = 1;
	dev->tag_set.queue_depth = 128;
	dev->tag_set.numa_node = NUMA_NO_NODE;
	dev->tag_set.cmd_size = 0;
	dev->tag_set.flags = BLK_MQ_F_SHOULD_MERGE;
	err = blk_mq_alloc_tag_set(&dev->tag_set);
	if (err) {
	    printk(KERN_ERR "blk_mq_alloc_tag_set: can't allocate tag set\n");
	    goto out_alloc_tag_set;
	}

	dev->queue = blk_mq_init_queue(&dev->tag_set);
	if (IS_ERR(dev->queue)) {
		printk(KERN_ERR "blk_mq_init_queue: out of memory\n");
		err = -ENOMEM;
		goto out_blk_init;
	}
	blk_queue_logical_block_size(dev->queue, RB_SECTOR_SIZE);
	dev->queue->queuedata = dev;

	dev->gd = alloc_disk(RB_MINOR_CNT);
	if (!dev->gd) {
		printk(KERN_ERR "alloc_disk: failure\n");
		err = -ENOMEM;
		goto out_alloc_disk;
	}

	dev->gd->major = rb_major;
	dev->gd->first_minor = 0;
	dev->gd->minors = RB_MINOR_CNT;
	dev->gd->fops = &my_block_ops;
	dev->gd->queue = dev->queue;
	dev->gd->private_data = dev;
	sprintf(dev->gd->disk_name, "dof");
	set_capacity(dev->gd, size);

	add_disk(dev->gd);

	return 0;

out_alloc_disk:
	blk_cleanup_queue(dev->queue);
out_blk_init:
	blk_mq_free_tag_set(&dev->tag_set);
out_alloc_tag_set:
	printk("out_alloc_tag_set");
out_vmalloc:
	return err;
}


static int __init my_dof_block_init(void)
{
	int err = 0;
	int ret;
	printk("in ram_block");
	if((ret = ramdevice_init())<0)
	{
		return ret;
	}
	int size = ret;
	rb_major = register_blkdev(rb_major, "rb");
	if (rb_major < 0) {
		printk(KERN_ERR "register_blkdev: unable to register\n");
		return err;
	}

	err = create_block_device(&g_dev,size);
	if (err < 0)
		goto out;

	return 0;

out:
	unregister_blkdev(rb_major, "rb");
	return err;
}

static void delete_block_device(struct my_block_dev *dev)
{
	if (dev->gd) {
		del_gendisk(dev->gd);
		put_disk(dev->gd);
	}

	if (dev->queue)
		blk_cleanup_queue(dev->queue);
	if (dev->tag_set.tags)
		blk_mq_free_tag_set(&dev->tag_set);
}

static void __exit my_dof_block_exit(void)
{
	delete_block_device(&g_dev);

	unregister_blkdev(rb_major, "rb");
	
	printk("exiting ram_Device");
}

module_init(my_dof_block_init);
module_exit(my_dof_block_exit);
