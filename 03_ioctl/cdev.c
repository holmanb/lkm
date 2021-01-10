#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include "cdev_priv.h"



/*Struct points to the device functions */
static struct file_operations file_ops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
    .unlocked_ioctl = device_ioctl,
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

static ssize_t device_write(struct file *file, const char *buffer, size_t len, loff_t *offset) {
    //printk(KERN_ALERT DEVICE_NAME "device_write(%p,%s,%d)", file, buffer, len);
    int bytes_written=0;
    while(bytes_written<len && bytes_written<MSG_BUFFER_LEN){
        get_user(msg_buffer[bytes_written], buffer+bytes_written); 
	bytes_written++;
    }
    msg_buffer[bytes_written] = '\0';
    
    return bytes_written;
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

/*ioctl*/
long int device_ioctl(
	struct file *file,
	unsigned int ioctl_num,
	unsigned long ioctl_param){
	
	int i;
	char ch;

	if(ioctl_num == IOCTL_SET_MSG){
		char * temp = (char*) ioctl_param;
		get_user(ch, temp);
		for(i=0; ch && i<MSG_BUFFER_LEN; i++, temp++){
			get_user(ch, temp);
		}
		device_write(file, (char *) ioctl_param, i, 0);
	}
	else if(ioctl_num == IOCTL_GET_MSG){
		i = device_read(file, (char*) ioctl_param, MSG_BUFFER_LEN-1, 0);
		put_user('\0', (char*) ioctl_param + i);
	}
	else if(ioctl_num == IOCTL_GET_NTH_BYTE){
		if(ioctl_param < MSG_BUFFER_LEN){
			return msg_buffer[ioctl_param];
		}
	}
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

