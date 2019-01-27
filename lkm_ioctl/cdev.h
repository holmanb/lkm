
#define DEVICE_NAME "cdev"
#define CLASS_NAME "example"
#define MSG_BUFFER_LEN 55 
#define EXAMPLE_MSG "42 is the meaning of life the universe and everything!\n"

// IOCTL calls
#define IOCTL_MAGIC 0x70
#define IOCTL_SET_MSG 		_IOR(IOCTL_MAGIC, 0, char *) 
#define IOCTL_GET_MSG 		_IOR(IOCTL_MAGIC, 1, char *) 
#define IOCTL_GET_NTH_BYTE 	_IOR(IOCTL_MAGIC, 2, int) 
