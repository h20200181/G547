/*Kernel module for IMU containing MPU9255 and BMP280 sensors*/
//The BMP280 sensor's Barometric pressure reading is 20 bits.
//The MPU9255 is a digital sensor with 16 bit ADC for conversions.
//Therefore the readings for acceleration, orientation and magnetic field is 16bits.


#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/time.h>
#include<linux/uaccess.h>
#include<linux/random.h>
#include "chardev.h"

#define ACC_Bytes 2
#define GYR_Bytes 2
#define MAG_Bytes 2
#define BP_Bytes 3

#define SUCCESS 0

static dev_t first; 
static struct cdev c_dev;
static struct class *cls;

static uint32_t i;
static int return_value,size_of_data, Device_Open = 0;
char address[6];

static int my_open(struct inode *i, struct file *f){
	
	#ifdef DEBUG
	printk(KERN_INFO "device opened(%p)\n",file);
	#endif
	
	if(Device_Open)
		return -EBUSY;
	
	Device_Open ++;
	
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int my_close(struct inode *i, struct file *f){
	
	#ifdef DEBUG
	printk(KERN_INFO "device released(%p,%p)\n",inode,file);
	#endif
	Device_Open --;
	module_put(THIS_MODULE);
	return SUCCESS;
}

static ssize_t my_read(struct file *f,char __user *buf, size_t len, loff_t *off){	

	get_random_bytes(&i,sizeof(i));
	if(size_of_data ==2)
		//MPU9255 sensor	
		i = i>>16;
		
	else 
		//BMP280 sensor
		i = i>>12;
		
	printk(KERN_INFO "imu_char device read()");
	return_value = copy_to_user(buf,&i,sizeof(i));
	return return_value;
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off){	
		
		return_value = copy_from_user(address,buf,len);
		printk(KERN_INFO "imu_char device write(), register address: %s\n",address);
		return return_value;
}

long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param){

	char * temp;
	temp = (char*)ioctl_param;
	switch(ioctl_num){
		case IOCTL_Accx:size_of_data = ACC_Bytes;
				my_write(file,temp,4,0);
				break;
		case IOCTL_Accy:size_of_data = ACC_Bytes;
				my_write(file,temp,4,0);
				break;
		case IOCTL_Accz:size_of_data = ACC_Bytes;
				my_write(file,temp,4,0);
				break;
		case IOCTL_Gyrx:size_of_data = GYR_Bytes;
				my_write(file,temp,4,0);
				break;
		case IOCTL_Gyry:size_of_data = GYR_Bytes;
				my_write(file,temp,4,0);
				break;
		case IOCTL_Gyrz:size_of_data = GYR_Bytes;
				my_write(file,temp,4,0);
				break;
		case IOCTL_Magx:size_of_data = MAG_Bytes;
				my_write(file,temp,4,0);
				break;
		case IOCTL_Magy:size_of_data = MAG_Bytes;
				my_write(file,temp,4,0);
				break;
		case IOCTL_Magz:size_of_data = MAG_Bytes;
				my_write(file,temp,4,0);
				break;
		case IOCTL_Bar: size_of_data = BP_Bytes;
				my_write(file,temp,6,0);	
				break;
		default: printk(KERN_ALERT "IOCTL SYS CALL ERROR");
	
	}
	return SUCCESS;
}

static struct file_operations fops={
	.owner	=THIS_MODULE,
	.open	=my_open,
	.release=my_close,
	.read	=my_read,
	.write	=my_write,
	.unlocked_ioctl = device_ioctl
};

static int __init imuchar_init(void){
	
	printk(KERN_INFO "Hello Linux\n");
	if (alloc_chrdev_region(&first,0,1,"imu_char_region")<0){
	return -1;
	}
	
	if((cls = class_create(THIS_MODULE,"imu_char_module")) == NULL){
		unregister_chrdev_region(first,1);
		return -1;
	}
	if(device_create(cls,NULL,first,NULL,"imu_char")==NULL){
		class_destroy(cls);
		unregister_chrdev_region(first,1);
		return -1;
	}
	
	cdev_init(&c_dev,&fops);
	if(cdev_add(&c_dev,first,1)==-1)
	{
		device_destroy(cls,first);
		class_destroy(cls);
		unregister_chrdev_region(first,1);
		return -1;
	}
	return 0;
}
 
static void __exit imuchar_exit(void)
{
	cdev_del(&c_dev);
	device_destroy(cls,first);
	class_destroy(cls);
	unregister_chrdev_region(first,1);
	printk(KERN_INFO "Bye Linux\n\n");
}


module_init(imuchar_init);
module_exit(imuchar_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Assignment 1/Char Driver");
MODULE_AUTHOR("Aakanksha Gujarathi, h20200181p");


