/*Header files*/
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/uaccess.h>


/*Function prototypes*/
static int __init drv_init(void); 
static void __exit drv_exit(void);
static int drv_open(struct inode *inode, struct file *filp);
static int drv_release( struct inode *inode, struct file *filp);
static ssize_t drv_read(struct file *file,char __user *buf ,size_t len,loff_t *off);
static ssize_t drv_write(struct file *file, const char *buf ,size_t len, loff_t *off);

/*a variable to store the device number */
dev_t dev=0; //device number = MKDEV(major,minor)
int baseminor=0,count=1;

static struct class *dev_class;


struct file_operations fops=
{
	.owner = THIS_MODULE,
	.read =drv_read,
	.write = drv_write,
	.open = drv_open,
	.release =drv_release,
};

static int drv_open(struct inode *inode ,struct file *filp)
{
	//lets do nothing here
	printk("\n open method for major %d and minor %d",imajor(inode),iminor(inode));
	return 0;

}

static int drv_release( struct inode *inode, struct file *filp)
{
	//lets do nothing here
	printk("\n release method for major %d and minor %d",imajor(inode),iminor(inode));
	return 0;
}

static ssize_t drv_read(struct file *file,char __user *buf ,size_t len,loff_t *off)
{
	//lets do nothing here
	printk("\n read method");
	return 0;
}

static ssize_t drv_write(struct file *file, const char *buf ,size_t len, loff_t *off)
{
	//lets do nothing here
	printk("\n write  method");
	return 0;
}

static int __init drv_init(void)
{
        int retval=0;
        /* registering the device using __register_chrdev API */
        retval= __register_chrdev(&dev,baseminor,count,"Srikar_Devicedrv",&fops);
        if(retval)
        {
            /* if failed just return the error number */
            printk(KERN_ALERT"device registration failed\n");
            goto reg_failed;
        }
        else
        {
            /* if succesfully registered print message in the log*/
            printk(KERN_ALERT"Srikar_Devicedrv registered succesfully retval =%d\n",retval);
        }
        
        dev_class= class_create(THIS_MODULE,"Srikar_class");
        /*how to know whether class_Create has failed
         * 1.use IS_ERR() to know whether the return value contains error or not
         * 2.use PTR_ERR() macro to know the error number
         */        
        if(IS_ERR(dev_class))
        {
           retval= PTR_ERR(dev_class);
           printk(KERN_ALERT"Srikar_class failed to create retval =%d\n",retval);
           goto cls_failed;
        }
        else
        {
            printk(KERN_ALERT"Srikar_class creation is succesfully \n");
        }
        /* Lets create device using device_create()
         * for now one device is fine.
         */
          
        retval=device_create(/*class*/dev_class, /*parent*/NULL,/*private data*/NULL,"%s","Srikar_device");
        if(retval)
        {
		goto devcrt_failed;
           printk(KERN_ALERT"Device Srikar_device creation failed retval =%d",retval);
	
            
        }
        else
        {
            
           printk(KERN_ALERT"Device Srikar_device is succesfully created retval =%d",retval);
        }
devcrt_failed:
    class_destroy(dev_class);
cls_failed:
    unregister_chrdev(MAJOR(dev),"Srikar_Devicedrv");
reg_failed:
	printk(KERN_ALERT"%d is the retval before init exits ",retval);
    return retval;
}

static void __exit drv_exit(void)
{
    printk(KERN_ALERT"Srikar_Devicedrv into exit function");
    /*first destroy the device*/
    device_destroy(dev_class, dev);
    /* then destroy the class */
    class_destroy(dev_class);
    /*last ,unregister the device*/
    unregister_chrdev(MAJOR(dev),"Srikar_Devicedrv");
}

module_init(drv_init);
module_exit(drv_exit);

/* module info */
MODULE_AUTHOR("SRIKAR SANA");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Srikar char device driver");
