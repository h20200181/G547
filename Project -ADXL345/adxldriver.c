#include <linux/kernel.h>      
#include <linux/module.h>      
#include <linux/fs.h>
#include <asm/uaccess.h>       
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/gpio.h>                
#include <linux/interrupt.h> 
#include <linux/workqueue.h>
#include <asm/uaccess.h>
#include "adxldrv.h"



//Register definitions
#define REG_DEVID 0x00
#define REG_DATA_FORMAT 0x31
#define REG_POWER_CTL 0x2D
#define REG_TIME_INACT 0x26
#define REG_THRESH_INACT 0x25
#define REG_BW_RATE 0x2C
#define REG_DATAX0 0x32
#define REG_DATAX1 0x33
#define REG_DATAY0 0x34
#define REG_DATAY1 0x35
#define REG_DATAZ0 0x36
#define REG_DATAZ1 0x37
#define SLAVE_ADDRESS 0x53


struct class *cl;
static struct i2c_client * my_client;
struct i2c_adapter * my_adapter;
static dev_t first;
static struct cdev c_dev;

static u8 adxl_read(struct i2c_client *client, u8 reg)
{
	int ret;
	ret = i2c_smbus_read_byte_data(client, reg); 
	if (ret < 0)
		dev_err(&client->dev,
			"can not read register, returned %d\n", ret);

	return (u8)ret;
}

static int adxl_write(struct i2c_client *client, u8 reg, u8 data)
{
	int ret;

	ret = i2c_smbus_write_byte_data(client, reg, data);
	if (ret < 0)
		dev_err(&client->dev,"can not write register, returned %d\n", ret);

	return ret;
}

static ssize_t my_adxl_read(struct file *f, char __user *buf, size_t len, loff_t *off) {	
	printk(KERN_INFO "Driver read()\n");
	return 0;
}


static int my_adxl_open(struct inode *i, struct file *f)
{
	  printk(KERN_INFO "Driver: open()\n");
	    return 0;
}
static int my_adxl_close(struct inode *i, struct file *f)
{
	  printk(KERN_INFO "Driver: close()\n");
	    return 0;
}

static ssize_t my_adxl_write(struct file *f, const char __user *buf,
		   size_t len, loff_t *off)
{
	  printk(KERN_INFO "Driver: write()\n");
	    return len;
}


long device_ioctl_adxl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param){

	u8 data,temp;
	switch(ioctl_num){
		case IOCTL_Accx0:data=adxl_read(my_client, REG_DATAX0);
				break;
		case IOCTL_Accx1 :data = adxl_read(my_client, REG_DATAX1);
				break;
		case IOCTL_Accy0:data =adxl_read(my_client, REG_DATAY0);
				break;
		case IOCTL_Accy1 :data = adxl_read(my_client, REG_DATAY1);
				break;
		case IOCTL_Accz0:data = adxl_read(my_client, REG_DATAZ0);
				break;
		case IOCTL_Accz1:data = adxl_read(my_client, REG_DATAZ1);
				break;
		case IOCTL_standby:temp = adxl_read(my_client,REG_POWER_CTL);
				   temp = temp & ~(1<<3);
				   adxl_write(my_client, REG_POWER_CTL, temp);
				break;
		case IOCTL_poweron:temp = adxl_read(my_client,REG_POWER_CTL);
				   temp = temp | (1<<3);
				   adxl_write(my_client, REG_POWER_CTL, temp);
				break;
		case IOCTL_sleep:temp = adxl_read(my_client,REG_POWER_CTL);
				   temp = temp | (1<<3) | (1<<2);
				   adxl_write(my_client, REG_POWER_CTL, temp);
				break;
		case IOCTL_BW_high:temp = adxl_read(my_client,REG_BW_RATE);
				   temp = temp | (0b00001111);
				   adxl_write(my_client, REG_POWER_CTL, temp);
				break;
		case IOCTL_BW_low:temp = adxl_read(my_client,REG_BW_RATE);
				   temp = temp | (0b1);
				   adxl_write(my_client, REG_POWER_CTL, temp);
				break;
		default: printk(KERN_ALERT "IOCTL SYS CALL ERROR");
			 data =0;
			 break;
	
	}
	copy_to_user((u8*)ioctl_param,&data, sizeof(data));
	
	return 0;
}
static struct file_operations my_dev_ops = {.read = my_adxl_read,
						.owner = THIS_MODULE,
						.open = my_adxl_open,
						.release = my_adxl_close,
						.write = my_adxl_write,
						.unlocked_ioctl = device_ioctl_adxl};

static int __init adxl345_init(void)
{
	
	u8 readvalue,temp; 
        printk(KERN_INFO "Welcome!\n");
       
        if (alloc_chrdev_region(&first, 0, 1, "adxlregion") < 0) {
        printk(KERN_DEBUG "Can't register device\n");
        return -1;
        }

	if (IS_ERR(cl = class_create(THIS_MODULE, "adxlclass"))){
			unregister_chrdev_region(first, 1);
			}

	if (IS_ERR(device_create(cl, NULL, first, NULL, "adxl345"))) {
		class_destroy(cl);
		unregister_chrdev_region(first,1);
	}

	cdev_init(&c_dev,&my_dev_ops);
	if (cdev_add(&c_dev, first,1) < 0)
	{
	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first,1);
	return -1;
	}

	my_adapter = i2c_get_adapter(1); 
	if (!(my_client = i2c_new_dummy_device(my_adapter, SLAVE_ADDRESS))){
		printk(KERN_INFO "Couldn't acquire i2c slave");
		device_destroy(cl, first);
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
	}
		
	readvalue = adxl_read(my_client,REG_DEVID);
	if (readvalue == (0b11100101)) {
		printk("Accelerometer detected, value = %d\r\n",readvalue);
	}

	//Setting Data format to be right justified
	temp = adxl_read(my_client,REG_DATA_FORMAT);
	temp = temp & (~(1<<2));
	adxl_write(my_client, REG_DATA_FORMAT, temp);
	
	
	//Start measurement
	temp = adxl_read(my_client,REG_POWER_CTL);
	temp = temp | (1<<3);
	adxl_write(my_client, REG_POWER_CTL, temp);

	return 0;
}
 

static void __exit adxl345_exit(void)
{
        printk(KERN_INFO "ADXL: Removing module");
 	
	i2c_unregister_device(my_client);
        cdev_del(&c_dev);
 	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first, 1); 
}


module_init(adxl345_init);
module_exit(adxl345_exit);

MODULE_DESCRIPTION("Driver for ADXL345");
MODULE_AUTHOR("Aakanksha and Anuj");
MODULE_LICENSE("GPL");


