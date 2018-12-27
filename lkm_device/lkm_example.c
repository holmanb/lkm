#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brett Holman");
MODULE_DESCRIPTION("Example Linux Kernel Module device driver.");
MODULE_VERSION("0.01");

#define DEVICE_NAME "lkm_example"
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

/*Struct points to the device functions */
static struct file_operations file_ops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

/* read function */
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
    printk(KERN_ALERT "This operation is not supported. \n");
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
    /*decrement the ope counter and usage count*/
    device_open_count--;
    module_put(THIS_MODULE);
    return 0;
}

/*init*/
static int __init lkm_example_init(void){
    strncpy(msg_buffer, EXAMPLE_MSG, MSG_BUFFER_LEN);
    msg_ptr = msg_buffer;

    /* try registering char device */
    major_num = register_chrdev(0, "lkm_example", &file_ops);
    if(major_num<0) {
        printk(KERN_ALERT "Could not register device: %d\n", major_num);
        return major_num;
    } else {
        printk(KERN_INFO "lkm_example module loaded with device major number %d\n", major_num);
        return 0;
    }
}

/*exit*/
static void __exit lkm_example_exit(void) {
    /*unregister*/
    printk(KERN_INFO "Goodbye, World!\n");
    unregister_chrdev(major_num, DEVICE_NAME);
    printk(KERN_INFO "Goodbye, World!\n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);

