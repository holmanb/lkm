obj-m += lkm_example.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	sudo rm /dev/lkm_example

test:
	-sudo rmmod lkm_example.ko
	sudo dmesg -C
	sudo insmod lkm_example.ko
	dmesg
	sudo mknod /dev/lkm_example c $$(dmesg | awk '{print $$NF}') 0
	echo "loaded module and created device file"
