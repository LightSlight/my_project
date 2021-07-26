#include "test.h"
#define FILE_PATH  "/mnt/part_sdc1/bitmap"

DECLARE_BITMAP(my_bitmap, BITMAP_BIT);

static int write_bitmap(struct file* fp, unsigned long block, unsigned long *bitmap) {
    loff_t pos;
    int res = 0;
    
    pos = BITMAP_BYTE * PART(block);
    res = kernel_write(fp, my_bitmap, sizeof(my_bitmap), pos); 
    // 第一个参数表示要写入的文件，第二个表示从缓冲区读数据；
    // 第三个参数表示读多少字节，第四个参数表示从文件的指定位置开始写。
    if (res < 0) {
        printk("kernel_write() error: %d\n", res);
        return -1;
    }
    
    return 0;
}

static void get_blknr(unsigned long block) {
    struct file *fp;
    loff_t read_pos;
    int res = 0;
    
    // 打开bitmap文件进行操作
    fp = filp_open(FILE_PATH, O_RDWR|O_CREAT,0777);
    if (IS_ERR(fp)){
        printk("create file error.\n");
        return ;
    }
    
    // 写bitmap之前先把相对应的那一块bitmap读出来进行修改
    read_pos = BITMAP_BYTE * PART(block);
    res =  kernel_read(fp, read_pos, my_bitmap, sizeof(my_bitmap)); 
    if(res != sizeof(my_bitmap)){
        printk("kernel_read() error:%d\n", res);       
        return ;
    }

    // 置相应的位为1
    set_bit(POSITION(block), my_bitmap);
    
    // 修改相应的bitmap
    res = write_bitmap(fp, block, my_bitmap);
    
    filp_close(fp, NULL);
}
EXPORT_SYMBOL(get_blknr);

static int __init test_init(void) {
    printk("module test loaded!\n");
    return 0;
}

static void __exit test_exit(void) {
    printk("module test unloaded!\n");
}

module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");
