#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>

#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/slab.h> // kernel free & kernel malloc

#define STRING_SIZE 1024    //my string size

MODULE_LICENSE("GPL");

static struct dentry *dir, *inputdir, *ptreedir;
static struct task_struct *curr;
static struct debugfs_blob_wrapper *myblob;
char *mystring;         // to save my string

static ssize_t write_pid_to_input(struct file *fp,
                                const char __user *user_buffer,
                                size_t length,
                                loff_t *position)
{
        pid_t input_pid;
        char buffer[STRING_SIZE];
        mystring[0] = '\0';
        sscanf(user_buffer, "%u", &input_pid);
        curr = pid_task(find_vpid(input_pid),PIDTYPE_PID);  //get tast_struct from input_pid
		if(curr==NULL)
		{
			printk("pid error occured");
			return length;
		}
        while(curr->pid != 1)
        {
            snprintf(buffer,STRING_SIZE,"%s (%d)\n",curr->comm,curr->pid);
            strncat(buffer,mystring,STRING_SIZE);
            strncpy(mystring,buffer,STRING_SIZE);
            curr = curr->parent;
            // get pid string and concatenate string to mystring
            // purpose of safety, I use snprintf not sprintf
            // because of bufferOverflow.
        }
        snprintf(buffer,STRING_SIZE,"%s (%d)\n",curr->comm,curr->pid);   //if pid==1, get last step
        strncat(buffer,mystring,STRING_SIZE);
        strncpy(mystring,buffer,STRING_SIZE);

        myblob->data = mystring;
        myblob->size = STRING_SIZE * sizeof(char);
        //setting myblob's data and size to mystring.

        // Find task_struct using input_pid. Hint: pid_task

        // Tracing process tree from input_pid to init(1) process

        // Make Output Format string: process_command (process_id)

        return length;
}

static const struct file_operations dbfs_fops = {
        .write = write_pid_to_input,
};

static int __init dbfs_module_init(void)
{
        // Implement init module code

        dir = debugfs_create_dir("ptree", NULL);
	int mystring_size;
	int struct_size;

	struct_size = sizeof(struct debugfs_blob_wrapper);
	mystring_size = STRING_SIZE * sizeof(char);

    if (!dir) {
                printk("Cannot create ptree dir\n");
                return -1;
        }

	mystring = (char *) kmalloc(mystring_size,GFP_KERNEL);
	if(mystring == NULL)
	{
		printk("Could not allocate mem for string\n");
		return -ENOMEM;
	}


	myblob = (struct debugfs_blob_wrapper *) kmalloc(struct_size,GFP_KERNEL);
	if(myblob == NULL)
	{
		printk("Could not allocate mem for blob\n");
		kfree(mystring);
		return -ENOMEM;
	}

	myblob->size = (unsigned long) mystring_size;
	myblob->data = (void *) mystring;

    inputdir = debugfs_create_file("input",0644,dir,NULL,&dbfs_fops);
    ptreedir = debugfs_create_blob("ptree",0644,dir,myblob); // Find suitable debugfs API
    // this statement save mystring to file.

    return 0;

}

static void __exit dbfs_module_exit(void)
{
        // Implement exit module code
    debugfs_remove_recursive(dir); // delete directory

    if(myblob) kfree(myblob);
	if(mystring) kfree(mystring);
    // free malloc
}

module_init(dbfs_module_init);
module_exit(dbfs_module_exit);
