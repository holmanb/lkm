#include "cdev.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brett Holman");
MODULE_DESCRIPTION("Example Linux Kernel Module character device driver.");
MODULE_VERSION("0.01");




/* prototypes*/
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
long int device_ioctl(
	struct file *file,
	unsigned int ioctl_num,
	unsigned long ioctl_param);

static int major_num;
static int device_open_count = 0;
static char msg_buffer[MSG_BUFFER_LEN];
static char *msg_ptr;

static struct class* charClass = NULL;
static struct device* charDevice = NULL;
