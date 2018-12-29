#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <asm/pgtable.h>


MODULE_LICENSE("GPL");

static struct dentry *dir, *output;
static struct task_struct *task;

struct packet{
        pid_t pid;
        unsigned long vaddr;
        unsigned long paddr;
};
/*
union test{
    unsigned long val;
    unsigned long *adr;
};
*/
static ssize_t read_output(struct file *fp,
                        char __user *user_buffer,
                        size_t length,
                        loff_t *position)
{
        struct packet *pac = (struct packet*) user_buffer; //get packet value

        task = pid_task(find_vpid(pac->pid),PIDTYPE_PID);  //get task_struct by pid

        unsigned long vaddress = pac->vaddr;
        unsigned long paddress = 0;

        unsigned long page_addr = 0;
        unsigned long page_offset = 0;

        struct mm_struct *mymm = task->mm;  //get mm_struct
        pgd_t *pgd;
        pud_t *pud;
        pmd_t *pmd;
        pte_t *pte;

        pgd = pgd_offset(mymm,vaddress); //get pgd's real address(pgd entry)
        if(pgd_none(*pgd))
        {
            printk("not mapped in pgd\n");
            return length;
        }

        pud = pud_offset(pgd, vaddress); //get pud's real address(pud entry)
        if(pud_none(*pud))
        {
            printk("not mapped in pud\n");
            return length;
        }

        pmd = pmd_offset(pud, vaddress); // get pmd's real address(pmd entry)
        if (pmd_none(*pmd)) {
            printk("not mapped in pmd\n");
            return length;
        }
        pte = pte_offset_kernel(pmd, vaddress);// get pte's real address(pte entry)
        if (pte_none(*pte)) {
            printk("not mapped in pte\n");
            return length;
        }

        page_addr = (pte_pfn(*pte) << PAGE_SHIFT); 
	// get physical address using pte and SHIFT for get PPN
        page_offset = vaddress & ~PAGE_MASK;
        paddress = page_addr | page_offset;     
	//finally, get physical address

        pac->paddr = paddress;

        //union test t;
        //t.val = pac->vaddr;
        //unsigned long realaddr = virt_to_phys(t.adr);
        //pac->paddr = *(t.adr);

        return length;
        // Implement read file operation
}

static const struct file_operations dbfs_fops = {
        .read = read_output,
        // Mapping file operations with your functions
};

static int __init dbfs_module_init(void)
{
        // Implement init module

        dir = debugfs_create_dir("paddr", NULL);

        if (!dir) {
                printk("Cannot create paddr dir\n");
                return -1;
        }
        // Fill in the arguments below
        output = debugfs_create_file("output",0666,dir,NULL,&dbfs_fops);

        return 0;
}

static void __exit dbfs_module_exit(void)
{
        debugfs_remove_recursive(dir);
        // Implement exit module
}

module_init(dbfs_module_init);
module_exit(dbfs_module_exit);
