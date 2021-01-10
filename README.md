kernel play
===========

*testing grounds for loadable kernel modules (LKM)*



### Install

	

**Fedora**

```
sudo dnf install git gcc make kernel-devel
git clone https://github.com/holmanb/lkm.git
sudo reboot -h now
```


### Usage


each directory contains a Makefile for running tests

each Makefile implements `all`/`clean`/`test`


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
