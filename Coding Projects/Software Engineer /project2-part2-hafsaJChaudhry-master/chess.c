#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define MODULE_NAME "Chess"
#define mem_size 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hafsa Chaudhry");
MODULE_DESCRIPTION("The character device driver");

//static dev_t dev;
dev_t device_number = 0;
static struct cdev the_cdev; /* type device aka character */
static struct class* the_class = NULL; /* class pointer */
uint8_t *kernel_buffer;

/**********************read and write**************************/
//in open return 0 if successful
static int chess_open(struct inode *pinode, struct file *pfile){
	/* Creating Physical Memory */
	if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0){
		printk(KERN_INFO "Cannot allocate the memory to the kernel..\n");
		return -1;
	}
	printk(KERN_INFO "Device File opened...\n");
	return 0;
}

//in read return how many bytes you read, initially return 0
static ssize_t chess_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset){
	copy_to_user(buffer, kernel_buffer, mem_size);
	printk(KERN_INFO "Data read : DONE...\n");
	return mem_size;

}

//in write you have to tell them how many bytes you are writing, initially return length
static ssize_t chess_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){
	copy_from_user(kernel_buffer, buffer, length);
	printk(KERN_INFO "Data is written successfully...\n");	
	return length;
}

//in release return 0 bc no error
static int chess_release(struct inode *pinode, struct file *pfile){
	kfree(kernel_buffer);
	printk(KERN_INFO "Device FILE closed...\n");
	return 0;
}
/*************************end**********************************/


//file_operations is a struct that contains pointers to structures
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = chess_read,
	.write = chess_write,
	.open = chess_open,
	.release = chess_release,
};


static int chess_init(void){	

	printk(KERN_ALERT "Module loaded tictactoe\n");

	//this will show under cat /proc/devices
	/* Allocating Major number */
	if(alloc_chrdev_region(&device_number, 0, 1, MODULE_NAME "_proc")<0){
		//if  error
		goto r_return;
	}
	
	printk(KERN_INFO"Major = %d Minor = %d..\n", MAJOR(device_number), MINOR(device_number));

	//get our class. This will show up under ls /sys/class
	the_class = class_create(THIS_MODULE, MODULE_NAME "_sys");
	if (the_class == NULL){
		printk(KERN_INFO "cannot create the struct class...\n");
		goto r_class;	
	}

	//in our proj, we will do just tictactoe instead of tictactoe_dev
	if(device_create(the_class, NULL, device_number, NULL, MODULE_NAME) == NULL){
		printk(KERN_INFO "cannot create the device ...\n");
		goto r_device;
	}

	/* creating cdev structure */
	cdev_init(&the_cdev, &fops);

	
	/* Adding character device to the system */
	if(cdev_add(&the_cdev, device_number, 1) < 0){
		goto r_cdev;
	}

	printk(KERN_INFO "Device driver insert done... properly....\n");
	return 0;

//in reverse order!!
r_cdev:
	printk(KERN_INFO "Cannot add the device to the system.\n");
	device_destroy(the_class, device_number);
	cdev_del(&the_cdev);
r_device:
	class_destroy(the_class);
r_class:
	unregister_chrdev_region(device_number, 1);
r_return:
	printk(KERN_INFO "Can't allocate the major number.\n");
	return -1;
}

static void chess_exit(void){
	printk(KERN_ALERT "Module unloaded tictactoe, byeeee\n");
	//cleanup in reverse order as well
	device_destroy(the_class, device_number);
	cdev_del(&the_cdev);
	class_destroy(the_class);
	unregister_chrdev_region(device_number, 1);
	printk(KERN_INFO " Device driver is removed successfully...\n");
}


module_init(chess_init);
module_exit(chess_exit);
