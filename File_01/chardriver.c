/*Header files*/
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/types.h>
#include<linux/cdev.h>
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
int major=0;
static struct class *dev_class =NULL;
static struct device *sr_device= NULL;

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
	printk(KERN_ALERT"\n open method for major %d and minor %d",imajor(inode),iminor(inode));
	return 0;

}

static int drv_release( struct inode *inode, struct file *filp)
{
	//lets do nothing here
	printk(KERN_ALERT"\n release method for major %d and minor %d",imajor(inode),iminor(inode));
	return 0;
}

static ssize_t drv_read(struct file *file,char __user *buf ,size_t len,loff_t *off)
{
	//lets do nothing here
	printk(KERN_ALERT"\n read method");
	return 0;
}

static ssize_t drv_write(struct file *file, const char *buf ,size_t len, loff_t *off)
{
	//lets do nothing here
	printk(KERN_ALERT"\n write  method");
	return 0;
}

static int __init drv_init(void)
{
        int retval=0;
        /*
         * registering the device using __register_chrdev API
         * We will be using dynamic allocation of major number by sending the major =0
         * If this is succesfull then it returns the major number or else we will get 
         * negative number.
         * So we shall write a longcode to know where it failed.
         * if it returns '0' then it means we did not got allocation of major number.
         */
        
        retval = __register_chrdev(major,baseminor,count,"sr_Devicedrv",&fops);
        
        if(retval<0)
        {
            /* if failed just return the error number */
            printk(KERN_EMERG"sr_Devicedriver device registration failed %d \n",retval);
            goto reg_failed;
        }
        else if(retval ==0)
        {
            /* if succesfully registered print message in the log*/
            printk(KERN_EMERG"sr_Devicedriver sr_Devicedrv registered succesfully retval =%d\n",retval);
            retval =-1;
            goto reg_failed;
        }
        else
        {
            /* the execution has come here then it means that we have reigstered our device*/
            major =retval;
            dev =MKDEV(major, 0);
            printk(KERN_ALERT"sr_Devicedriver registered succesfully with a major %d",major);
            retval=0; /*Intialize it again*/
        }
        
        dev_class= class_create(THIS_MODULE,"sr_class");
        /*how to know whether class_Create has failed
         * 1.use IS_ERR() to know whether the return value contains error or not
         * 2.use PTR_ERR() macro to know the error number
         */        
        if(IS_ERR(dev_class))
        {
           retval= PTR_ERR(dev_class);
           printk(KERN_EMERG"sr_Devicedriver sr_class failed to create retval =%d\n",retval);
           goto cls_failed;
        }
        else
        {
            printk(KERN_EMERG"sr_Devicedriver sr_class creation is succesfully \n");
        }
        /* Lets create device using device_create()
         * for now one device is fine.
         */
          
        sr_device =device_create(/*class*/dev_class, /*parent*/NULL, /*device number*/ dev, /*private data*/ NULL, "srdevice" );
        if(IS_ERR(sr_device))
        {
           retval= PTR_ERR(sr_device);
           printk(KERN_EMERG"sr_Devicedriver sr_class failed to create retval =%d\n",retval);
           goto devcrt_failed;
        }
        else
        {
            printk(KERN_EMERG"sr_Devicedriver sr_class creation is succesfully \n");
            goto success;
	}
        
devcrt_failed:
    class_destroy(dev_class);
cls_failed:
    __unregister_chrdev(major,0,count,"sr_Devicedrv");	
reg_failed:
	printk(KERN_EMERG"sr_Devicedriver %d is the retval before init exits ",retval);
success:
    return retval;
}

static void __exit drv_exit(void)
{
    printk(KERN_EMERG"sr_Devicedriver sr_Devicedrv into exit function");
    /*first destroy the device*/
    device_destroy(dev_class, dev);
    /* then destroy the class */
    class_destroy(dev_class);
    /*last ,unregister the device*/
    __unregister_chrdev(major,0,count,"sr_Devicedrv");
}

module_init(drv_init);
module_exit(drv_exit);

/* module info */
MODULE_AUTHOR("sr SANA");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("sr char device driver");
