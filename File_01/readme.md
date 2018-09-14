## 1.1 Introduction
The direcotry File_01 contains only registering of the char drivers but not using the most widely known api's , register_chrdev_region(),
alloc_chrdev_region(). Instead we are going to register the char device driver using __register_chrdev().

## what does __register_chrdev() does ?

I was wondering how register_chrdev_region() works so out of my curiosity i went and had look into the kernel source 	using lxr, a one hell of a tool which will do your search very easy. 
Normally we register a device using register_chrdev_region()  with a major number , baseminor and the number of minors we need from the baseminor or alloc_chrdev_region() if we want our major number to be allocated dynamically.
Next we allocate cdev structure using kmalloc as it is shown in ldd3 book.
When i was searching the linux source i found out that there is one API which will register a device if major number , baseminor and the number of minors given and also if we want our major number to be dynamically allocated then we can tell that api that our major number is '0'. If it recieves as zero it will dynamically allocates a major number . Apart from doing this it will also create a cdev, assign fops and owner attributes of the cdev automatically for us. So instead of doing multiple things its just a call to one API which will do all the above steps for us. 

#### Note: I have taken the below comments from the linux source itself

 * __register_chrdev() - create and register a cdev occupying a range of minors
 * @major: major device number or 0 for dynamic allocation
 * @baseminor: first of the requested range of minor numbers
 * @count: the number of minor numbers required
 * @name: name of this range of devices
 * @fops: file operations associated with this devices
 *
 * If @major == 0 this functions will dynamically allocate a major and return
 * its number.
 *
 * If @major > 0 this function will attempt to reserve a device with the given
 * major number and will return zero on success.
 *
 * Returns a -ve errno on failure. 
 *
 * The name of this device has nothing to do with the name of the device in
 * /dev. It only helps to keep track of the different owners of devices. If
 * your module name has only one type of devices it's ok to use e.g. the name
 * of the module here.

 
#### int __register_chrdev(unsigned int major, unsigned int baseminor, unsigned int count, const char *name, 
 ####  const struct file_operations *fops);

#### My one extra comment on this api: this returns major number on success.

So for my practice i have written a device driver module to learn this. 
You can also use this for your reference.
I am not sure whether we can use in this fashion or not but still comments are very much welcomed.
Want to comment please write to srikarsana1995@gmail.com.

So here comes some notes out of my experience now out of my mistakes.
I read ldd3 book before writing this. So, normally, in the exit function we use unregister_chrdev_region() api to unregister the 
device driver.
But in this case this api wont work out.
When registering ,normally according to the ldd3 book, we allocate memory to a cdev structure , assign the driver file_operations to it and then add the cdev to the system.
The api __register_chrdev() will do the registering and cdev stuff for us. so if we use unregister_chrdev_region() it will be little difficult for us as we cannot free cdev allocated.

So we have to be using __unregister_chrdev() which will make our life easy.
If you wish to have a look at my code..... proceed.. (for now see only init and exit methods. the rest are under construction ).




