#include "chess.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#define MODULE_NAME "chess"
#include "chess.h"


static dev_t device_number;
static struct cdev the_cdev;
static struct class* the_class = NULL;

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = chess_read,
	.write = chess_write,
	.open = chess_open,
	.release = chess_release
};

static int my_dev_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	add_uevent_var(env, "DEVMODE=%#o", 0666);
	return 0;
}

static int __init chess_start(void)
{
	printk(KERN_INFO "[" MODULE_NAME "] Module Loaded\n");

	if (alloc_chrdev_region(&device_number, 0, 1, MODULE_NAME) < 0)
	{
		goto r_return;
	}

	the_class = class_create(THIS_MODULE, MODULE_NAME);
	if (the_class == NULL)
	{
		goto r_class;
	}

	the_class->dev_uevent = my_dev_uevent;

	if(device_create(the_class, NULL, device_number, NULL, MODULE_NAME) == NULL)
	{
		goto r_device;
	}

	cdev_init(&the_cdev, &fops);

	if (cdev_add(&the_cdev, device_number, 1) < 0)
	{
		goto r_cdev;
	}

	return 0;
r_cdev:
	device_destroy(the_class, device_number);
	cdev_del(&the_cdev);
r_device:
	class_destroy(the_class);
r_class:
	unregister_chrdev_region(device_number, 1);
r_return:
	return -1;

}

static void __exit chess_end(void)
{
	printk(KERN_INFO "[" MODULE_NAME "] Module Unloaded\n");

	device_destroy(the_class, device_number);
	cdev_del(&the_cdev);
	class_destroy(the_class);
	unregister_chrdev_region(device_number, 1);
}

module_init(chess_start);
module_exit(chess_end);
MODULE_LICENSE("GPL");
