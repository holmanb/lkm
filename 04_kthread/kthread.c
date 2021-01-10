#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/slab.h>

#define NUM_THREADS 2

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brett Holman");
MODULE_DESCRIPTION("example Linux Kernel Module.");
MODULE_VERSION("0.01");


static struct task_struct **t;

int threaded_counter(void *p){
    int id = *((int *)p);
    while(!kthread_should_stop()){
	printk(KERN_INFO "Hello from kthread %d\n",id);
	msleep(1000);
    }
    return 0;
}

static int __init lkm_example_init(void){
    int kthread_num;
    printk(KERN_INFO "kthread test module loaded \n");
    t = kmalloc(sizeof(struct task_struct),GFP_KERNEL);
    
    //requires error checking
    for(kthread_num=0; kthread_num<NUM_THREADS; kthread_num++){
	    t[kthread_num] = kthread_create(threaded_counter, &kthread_num, "threaded_counter");
	    wake_up_process(t[kthread_num]);
	    printk(KERN_INFO "kthread %d spawned\n",kthread_num);
    }
    return 0;
}

static void __exit lkm_example_exit(void) {
    int kthread_num;
    for(kthread_num=0; kthread_num<NUM_THREADS; kthread_num++){
        kthread_stop(t[kthread_num]);
	printk(KERN_INFO "kthread %d stopped\n",kthread_num);
    }
    kfree(t);
    printk(KERN_INFO "kthread test module unloaded \n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);
