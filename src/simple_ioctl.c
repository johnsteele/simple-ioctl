/*
 * simple_ioctl.c -- This is a simple char driver that demonstrates the 
 *                   handling of ioctl system calls. Concurrency is also
 *                   taken into consideration when accessing resources.
 *
 * Copyright (C) John Steele <programjsteele@gmail.com>
 * 
 * $Id: simple_ioctl.c, v 1.0.0 2010/09/30 John Exp $
 */

#include <linux/module.h>
#include <linux/init.h>      

#include <linux/kernel.h>     /* printk() */
#include <linux/types.h>      /* size_t   */
#include <linux/cdev.h>       /* cdev     */
#include <linux/fs.h>         /* file_ops */
#include <linux/moduleparam.h>/* cdm args */
#include <linux/wait.h>       /* wait queue */
#include <linux/sched.h>      /* current  */

#include "simple_ioctl.h"     /* definitions */


/*
 * The name of this device driver.
 * Gets placed in the /proc/devices file.
 */
static char driver_name[] = "simple_ioctl_mod";


/*
 * The dynamically allocated char devices.
 * See device_init() for initialization.
 */
static struct cdev *my_devices;


/*
 * Optionally set at the command line.
 */
int my_major = MY_MAJOR;
int my_minor = MY_MINOR;
int my_num_devices = MY_NUM_DEVICES;

module_param (my_major, int, S_IRUGO);
module_param (my_minor, int, S_IRUGO);
module_param (my_num_devices, int, S_IRUGO);



/* 
 * Read from the device.
 */
ssize_t device_read (struct file *filp, char __user *buf, size_t count,
			loff_t *f_pos) 
{
	int transfered_bytes;
	transfered_bytes = 0;
	return transfered_bytes;
}


/*
 * Write to the device.
 */ 
ssize_t device_write (struct file *filp, const char __user *buf, size_t count,
			loff_t *f_pos)
{
	int transfered_bytes;
	transfered_bytes = 0;
	return transfered_bytes;
}


/* 
 * The operations that can be perfomed on the devices.
 */
struct file_operations fops = {
	.owner = THIS_MODULE,	
	.read  = device_read,
	.write = device_write,
};


/*
 * Initializes and sets up the provided cdev.
 *
 * @the_index The index the_cdev is in my_devices.
 */ 
static void setup_cdev(struct cdev *the_cdev, const int the_index)
{
	int dev_number, result;

	result = 0;
	dev_number = MKDEV(my_major, my_minor + the_index);

	cdev_init(the_cdev, &fops);	
	the_cdev->owner = THIS_MODULE;
	the_cdev->ops   = &fops;
	result = cdev_add(the_cdev, dev_number, 1);
	if (result < 0) {
		printk (KERN_NOTICE "Error registering cdev (%s%d).\n", 	
				driver_name, the_index);
 	}
}


/* 
 * Cleans up and releases resources.
 */
static void __exit device_cleanup(void) 
{
	int i;
	if (my_devices) {
		for (i = 0; i < my_num_devices; i++) {
			cdev_del(&my_devices[i]);
		}			
		kfree(my_devices);
	}
	unregister_chrdev_region(MKDEV(my_major, my_minor), my_num_devices);
}


/*
 * Initializes this device driver.
 */
static int __init device_init(void) 
{
	int result, i;
	dev_t dev;
	result = 0;

	if (my_major) {
		dev = MKDEV(my_major, my_minor);
		result = register_chrdev_region(dev, my_num_devices, driver_name); 	
	} else {
		result = alloc_chrdev_region(&dev, my_minor, my_num_devices, driver_name);
		my_major = MAJOR(dev);		
	}

	if (result < 0) {
		printk(KERN_NOTICE "Error getting major for (%s)\n", driver_name);
		return result;
	}

	my_devices = kmalloc(my_num_devices * sizeof (struct cdev), GFP_KERNEL);
	if (!my_devices) {
		printk (KERN_NOTICE "Not enough memory for (%d) devices.\n", my_num_devices);
		device_cleanup();
		return -ENOMEM;
	}

	memset (my_devices, 0, my_num_devices * sizeof (struct cdev));

	for (i = 0; i < my_num_devices; i++) {
		setup_cdev(&my_devices[i], i);
	}

	return 0;
} 


module_init(device_init);
module_exit(device_cleanup);

MODULE_LICENSE("DUAL BSD/GPL");
MODULE_AUTHOR ("John Steele");


