#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brett Holman");
MODULE_DESCRIPTION("Example Linux Kernel Module character device driver.");
MODULE_VERSION("0.01");

#define DEVICE_NAME "cdev"
#define CLASS_NAME "example"

#define EXAMPLE_MSG "42 is the meaning of life the universe and everything!\n"
#define MSG_BUFFER_LEN 55 

/* prototypes*/
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static int major_num;
static int device_open_count = 0;
static char msg_buffer[MSG_BUFFER_LEN];
static char *msg_ptr;

static struct class* charClass = NULL;
static struct device* charDevice = NULL;


/*Struct points to the device functions */
static struct file_operations file_ops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

/*read*/
static ssize_t device_read(struct file *flip, char *buffer, size_t len, loff_t *offset){
    int bytes_read = 0;
    /*if at end, loop to start*/
    if(*msg_ptr == 0) {
        msg_ptr = msg_buffer;
    }
    
    /*copy to buffer*/
    while(len && *msg_ptr) {
        put_user(*(msg_ptr++), buffer++);
        len--;
        bytes_read++;
    }
    return bytes_read;
}

/*read only*/
static ssize_t device_write(struct file *flip, const char *buffer, size_t len, loff_t *offset) {
    printk(KERN_ALERT DEVICE_NAME "This operation is not supported. \n");
    return -EINVAL;
}

/*open*/
static int device_open(struct inode *inode, struct file *file) {

    if(device_open_count) {
        return -EBUSY;
    }
    device_open_count++;
    try_module_get(THIS_MODULE);
    return 0;
}

/*release*/
static int device_release(struct inode *inode, struct file *file) {

    /*decrement the open counter and usage count*/
    device_open_count--;
    module_put(THIS_MODULE);
    return 0;
}

/*init*/
static int __init lkm_example_init(void){
    strncpy(msg_buffer, EXAMPLE_MSG, MSG_BUFFER_LEN);
    msg_ptr = msg_buffer;

    /* try registering char device */
    major_num = register_chrdev(0, DEVICE_NAME, &file_ops);
    if(major_num<0) {
        printk(KERN_ALERT DEVICE_NAME ": could not register device: %d\n", major_num);
        return major_num;
    } 
    printk(KERN_INFO DEVICE_NAME ": module loaded with device major number %d\n", major_num);

    /* register device class */
    charClass = class_create(THIS_MODULE,CLASS_NAME);
    if(IS_ERR(charClass)){
        unregister_chrdev(major_num, DEVICE_NAME);
	printk(KERN_ALERT DEVICE_NAME ": Failed to register device class\n");
	return PTR_ERR(charClass);
    }
    printk(KERN_INFO DEVICE_NAME ": class registered correctly\n");

    /* register device driver */
    charDevice = device_create(charClass, NULL, MKDEV(major_num, 0), NULL, DEVICE_NAME);
    if (IS_ERR(charDevice)){
        class_destroy(charClass);
	unregister_chrdev(major_num, DEVICE_NAME);
	printk(KERN_ALERT DEVICE_NAME "Failed to create the device\n");
	return PTR_ERR(charDevice);
    }
    printk(KERN_INFO DEVICE_NAME ": device class created correctly\n");
    return 0;
}


/*unregister*/
static void __exit lkm_example_exit(void) {

    device_destroy(charClass, MKDEV(major_num, 0));
    class_unregister(charClass);
    unregister_chrdev(major_num, DEVICE_NAME);
    printk(KERN_INFO DEVICE_NAME ": Module unloaded\n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);

