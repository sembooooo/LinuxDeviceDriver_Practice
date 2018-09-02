/*Header files*/

#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/uacess.h>

/*Function prototypes*/
static int __init drv_init(void); 
static void __exit drv_exit(void);
static int drv_open(struct inode *inode, struct file *filp);
static int drv_release( struct inode *inode, struct file *filp);
static int drv_read(struct file *file,char __user *buf ,size_t len,loff_t *off);
static int drv_write(struct file *file, const char *buf ,size_t len, loff_t *off);

/*a variable to store the device number */
dev_t dev=0; //device number = MKDEV(major,minor)

static struct class *dev_class;


struct file_operation fops=
{
	.owner = THIS_MODULE,
	.read =drv_read,
	.write =drv_write,
	.open =drv_open,
	.release =drv_release,
};

static int drv_open(struct inode *inode ,struct file *filp)
{
	//lets do nothing here
	printk("\n open method for major %d and minor %d",imajor(inode),iminor(inode));
	return 0;

}

static int drv_release( struct inode *inode, struct file *filp);
{
	//lets do nothing here
	printk("\n release method for major %d and minor %d",imajor(inode),iminor(inode));
	return 0;
}

static int drv_read(struct file *file,char __user *buf ,size_t len,loff_t *off);
{
	//lets do nothing here
	printk("\n read method for major %d and minor %d",imajor(inode),iminor(inode));
	return 0;
}

static int drv_write(struct file *file, const char *buf ,size_t len, loff_t *off);
{
	//lets do nothing here
	printk("\n read method for major %d and minor %d",imajor(inode),iminor(inode));
	return 0;
}

static int __init drv_init(void)
{
	

}

static void __exit drv_exit(void)
{

}



