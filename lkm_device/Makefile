NAME=cdev
UDEV=99-$(NAME).rules
obj-m += $(NAME).o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	sudo rmmod $(NAME)
	sudo rm /etc/udev/rules.d/$(UDEV)	

install: 
	# order is important, on fedora 29 at least, rules aren't retroactively applied
	sudo cp $(UDEV) /etc/udev/rules.d
	sudo insmod $(NAME).ko

test:
	-sudo rmmod $(NAME).ko
	sudo dmesg -C
	sudo insmod $(NAME).ko
	dmesg
	echo "loaded module [$(NAME)]  and created device file /dev/$(NAME)"

