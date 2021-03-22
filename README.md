kernel play
===========

*testing grounds for loadable kernel modules (LKM)*



### Dependencies

**Fedora**

```
sudo dnf install git gcc make kernel-devel
```

**Arch**

```
sudo pacman -S git gcc make linux-headers
```


### Install

git clone https://github.com/holmanb/lkm.git

### Usage

each directory contains a Makefile for running tests

each Makefile implements `all`/`clean`/`test`

to quickly test out modules in a VM, one can get up and running quickly with u-root + qemu + kvm


#### Usage
```
# initial setup
go get github.com/u-root/u-root

cd <project directory>
make all
u-root -files <kernel_module>.ko  core boot
qemu-system-x86_64 -kernel /boot/vmlinuz-$(uname -r) -initrd /tmp/initramfs.linux_amd64.cpio
...
<in vm>
...
insmod <kernel_module>.ko
...
<interact with module>
...
rmmod <kernel_module>.ko
```


### Resources

Source of Truth

https://github.com/torvalds/linux


Very Dated (2.6) But Valuable

http://tldp.org/LDP/lkmpg/2.6/html/index.html


A Short Character Device Blog Post

https://blog.sourcerer.io/writing-a-simple-linux-kernel-module-d9dc3762c234


Blog Post By Dr. Derek Molloy of Dublin City University, Ireland

www.derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction


Register A Device Class and Driver of a Dynamically Allocated Major Number From Kernel Space

www.derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device


Shows How To Add Ioctl To Char Device

https://www.tldp.org/LDP/lkmpg/2.4/html/x856.html
