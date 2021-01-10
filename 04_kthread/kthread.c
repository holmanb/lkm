#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#define THREAD_ID_1 2

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brett Holman");
MODULE_DESCRIPTION("example Linux Kernel Module.");
MODULE_VERSION("0.01");


static struct task_struct *t;

int threaded_counter(void *p){
    while(!kthread_should_stop()){
	printk(KERN_INFO "Hello from kthread\n");
	msleep(1000);
    }
    return 0;
}

static int __init lkm_example_init(void){
    printk(KERN_INFO "Hello, galaxy!\n");
    
    //requires error checking
    t = kthread_create(threaded_counter, NULL, "threaded_counter");
    wake_up_process(t);
    printk(KERN_INFO "kthread running successfully\n");
    return 0;
}

static void __exit lkm_example_exit(void) {
    kthread_stop(t);
    printk(KERN_INFO "Goodbye, galaxy!\n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);
