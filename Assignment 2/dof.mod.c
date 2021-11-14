#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x972f93ab, "module_layout" },
	{ 0x9d5669a1, "blk_cleanup_queue" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0xafed3b15, "blk_mq_start_request" },
	{ 0x720a27a7, "__register_blkdev" },
	{ 0x999e8297, "vfree" },
	{ 0x97651e6c, "vmemmap_base" },
	{ 0x5df54c04, "__alloc_disk_node" },
	{ 0x31e2a770, "blk_mq_init_queue" },
	{ 0xc5850110, "printk" },
	{ 0xb4d662d1, "del_gendisk" },
	{ 0x6101e0e7, "blk_mq_free_tag_set" },
	{ 0x5301de1c, "set_capacity" },
	{ 0xb5a459dc, "unregister_blkdev" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0x20debf6f, "put_disk" },
	{ 0x6849c681, "blk_mq_alloc_tag_set" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x52cdb49c, "blk_mq_end_request" },
	{ 0x69acdf38, "memcpy" },
	{ 0xf386b6f2, "device_add_disk" },
	{ 0xe517d867, "blk_queue_logical_block_size" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "D720FA23C4B3D9110961CE1");
