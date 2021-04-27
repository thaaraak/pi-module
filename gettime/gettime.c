#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>


int len;

char *msg;
ssize_t size;
struct timespec64 ts;

ssize_t read_proc(struct file *filp,char *buf,size_t count,loff_t *offp ) 
{
	struct timespec64 ts;
	char *temp;
	ssize_t ret;

	temp=kmalloc(50*sizeof(char),GFP_KERNEL);
	ktime_get_ts64(&ts);
	sprintf(temp,"%lld seconds \n%ld nanoseconds\n",ts.tv_sec, ts.tv_nsec);
	len=strlen(temp);
	size=sizeof(char)*len;
	
	ret = simple_read_from_buffer(buf,count,offp,temp,size);
	kfree(temp);

	return ret;
}
/*
struct file_operations proc_fops = {
	read:	read_proc
};
*/

struct proc_ops proc_fops = {
	proc_read:	read_proc
};


void create_new_proc_entry(void) 
{
	proc_create("gettime",0,NULL,&proc_fops);
}


int proc_init (void) {
	create_new_proc_entry();
	return 0;
}

void proc_cleanup(void) {
	remove_proc_entry("gettime",NULL);
}

MODULE_LICENSE("GPL");	
module_init(proc_init);
module_exit(proc_cleanup);

