#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define FN "./test.txt"

static struct file* fp;
static struct file* wfp;

static int __init init_rw(void)
{
    printk("%s()\n", __FUNCTION__);
    
    fp = filp_open(FN, O_RDONLY, 0);
    printk("File address:0x%p\n", fp);
    msleep(100);
    if (IS_ERR(fp))
    {
        printk("cannot open file.\n");
        goto FS_END;
    }

    char* out_file_name;
    const int of_len = strlen(FN) + 5;
    out_file_name = kmalloc(of_len, GFP_KERNEL);
    if (out_file_name == NULL)
    {
        printk("cannot malloc.\n");
        goto FS_END;
    }
    memset(out_file_name, 0, of_len);

    snprintf(out_file_name, of_len, "%s%s", FN, "_out");
    printk("out_file_name:%s\n", out_file_name);
    wfp = filp_open(out_file_name, O_WRONLY|O_CREAT, 0666);
    msleep(100);
    if (IS_ERR(wfp))
    {
        printk("cannot open the write file.\n");
        wfp = NULL;
    }

    mm_segment_t old_fs;
    old_fs = get_fs();
    set_fs(KERNEL_DS);

    int size = 0;
    char rbuf[6];
    loff_t pos = 16;
    loff_t wpos = 0;
    while (1)
    {
        memset(rbuf, 0, 6);
        // size表示读出的字节数    
        size = vfs_read(fp, rbuf, 6, &pos);
        printk("read ret:%d, pos:%lld\n", size, pos);
        if (size < 1)
        {
            printk("read end.\n");
            break;
        }

        printk("\t%s\n", rbuf);

        if (wfp)
        {
            size = vfs_write(wfp, rbuf, size, &wpos);
            printk("write ret:%d, pos:%lld\n\n", size, wpos);
        }
    }
    set_fs(old_fs);

    msleep(50);

FS_END:

    return 0;
}

static void __exit exit_rw(void)
{
    int ret = 0;
    if (!IS_ERR(fp))
    {
        printk("closing fs file.\n");
        ret = filp_close(fp, NULL);
        printk("close ret:%d\n", ret);
    }

    if (wfp && !IS_ERR(wfp))
    {
        printk("closing wfp.\n");
        ret = filp_close(wfp, 0);
        printk("close wfp ret:%d\n", ret);
    }
    msleep(100);
}

module_init(init_rw);
module_exit(exit_rw);
MODULE_LICENSE("GPL");
