obj-m += lkm_example.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test:
	sudo dmesg -C
	-sudo insmod lkm_example.ko
	sleep 5
	sudo rmmod lkm_example.ko
	dmesg
