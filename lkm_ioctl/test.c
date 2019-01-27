#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "cdev.h"

#define DEV_FILE "/dev/"DEVICE_NAME

// parts of this adopted from https://www.tldp.org/LDP/lkmpg/2.4/html/x856.html
void ioctl_set_msg(int file_desc, char *message){
	int ret_val;
	ret_val = ioctl(file_desc, IOCTL_SET_MSG, message);
	if(ret_val < 0){
		printf("ioctl_set_msg failed: %d", ret_val);
	}
}

char * ioctl_get_msg(int file_desc){
	int ret_val;
	char * message = malloc(MSG_BUFFER_LEN+1);
	if(!message) {
		printf("failed to allocated memory\n");
		exit(EXIT_FAILURE);
	}
	ret_val = ioctl(file_desc, IOCTL_GET_MSG, message);
	if(ret_val < 0){
		printf("ioctl_get_msg failed:%d\n", ret_val);
	}
	return message;
}

char ioctl_get_nth_byte(int file_desc,int n){
	int i=0;
	char c=ioctl(file_desc, IOCTL_GET_NTH_BYTE, n++);
	if(c<0){
		printf("ioctl_get_nth_byte failed at the %n'th byte\n",n);
		exit(EXIT_FAILURE);
	}
	return c;
}

int main(){
		
	char * line = NULL;
	char * new_msg = "oh look, the message changed\n";
	size_t len = 0;
	FILE * fp_r, *fp_w;
       	
	// open the device file
	fp_r = fopen(DEV_FILE, "r");
       	if(fp_r==NULL){
		printf("Failed to open file\n");
		exit(EXIT_FAILURE);
	}	 

	// read the device file
	getline(&line, &len, fp_r);
	//printf("output of "DEV_FILE":%s",line);

	// validate that it prints what it should
	if(strcmp(EXAMPLE_MSG, line)){
		printf("something is wrong.... very wrong\n");
	}

	// single access at a point in time (add concurrent read later?)
	fclose(fp_r);

	// write to the device file
	fp_w = fopen(DEV_FILE, "w");
       	if(fp_w==NULL){
		printf("Failed to open file\n");
		exit(EXIT_FAILURE);
	}	 
	fprintf(fp_w, new_msg);
	fclose(fp_w);

	// read the new value from the device file
	fp_r = fopen(DEV_FILE, "r");
       	if(fp_r==NULL){
		printf("Failed to open file\n");
		exit(EXIT_FAILURE);
	}	 
	getline(&line, &len, fp_r);

	// validate that it prints what it should
	if(strcmp(new_msg, line)){
		printf("something is wrong.... very wrong\n");
	}
	//printf("modified output of "DEV_FILE":%s",line);


	fclose(fp_r);
	
	if(line) free(line);

	printf("read/write tests passed\n");

	/// IOCTL tests
	int file_desc, ret_val;
	char *msg = "Message passed by ioctl\n";
	file_desc = open(DEV_FILE, O_RDWR);
	if(file_desc < 0){
		printf("Can't open file: "DEV_FILE);
		exit(EXIT_FAILURE);
	}
	ioctl_set_msg(file_desc, msg);
	if(strcmp(msg, ioctl_get_msg(file_desc))){
		printf("something is wrong.... very wrong\n");
	}
	
	if(ioctl_get_nth_byte(file_desc, 1) != 'e'){
		printf("something is wrong.... very wrong\n");
	}
	printf("ioctl tests passed\n");

	

	return 0;
}
