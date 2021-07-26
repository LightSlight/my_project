#include "dev.h"

static struct block_device *srcblk;
unsigned long blk_nr = 0;

extern void get_blknr(unsigned long);
static void filter_srcdev_registe(int major, int mimor);//过滤器注册，用于过滤原始设备bio
static void filter_srcdev_cancel(void);
static void (*make_req_fn_org)(struct request_queue*, struct bio*);//定义函数指针用于保存原始函数地址

static void send_blknr(unsigned long sector) {
    unsigned long blknr = sector / 8;
    get_blknr(blknr);
}

/*
 * 作用：源设备的请求处理函数调换
 * 参数：q,bio
 * 返回：
 */
static void source_req_fn_hook(struct request_queue* q, struct bio* bio){
    if(bio_data_dir(bio) == WRITE){
        printk("WRITE\n");
        // 分析bio数据
        printk(KERN_DEBUG "%s: bio->sector = [%lu],bi->size = [%u]\n",current->comm,bio->bi_sector,bio->bi_size); 
        blk_nr = bio->bi_sector;
        (*make_req_fn_org)(q, bio);
        send_blknr(blk_nr);
    }
    else{
        (*make_req_fn_org)(q, bio);
    }
    return;
}

/* 
 * 参数：源设备主设备号，次设备号
 * 作用：对源设备进行bio过滤
 * 返回：
 */
static void filter_srcdev_registe(int major, int minor){
    struct gendisk* gd1 = NULL;
    srcblk = blkdev_get_by_dev(MKDEV(major, minor),FMODE_READ |FMODE_WRITE,NULL);
    if(IS_ERR(srcblk)){
        printk("No such block device!\n");
        return;
    }
    gd1 = srcblk->bd_disk;
    make_req_fn_org = gd1->queue->make_request_fn;
    gd1->queue->make_request_fn = source_req_fn_hook;  //hook替换
    return;
}

/*
 * 作用：过滤器的取消
 *
 */
static void filter_srcdev_cancel(void){
    if((srcblk->bd_disk->queue->make_request_fn != NULL) && (make_req_fn_org != NULL)){
        srcblk->bd_disk->queue->make_request_fn = make_req_fn_org;
    }
}

/*
 * 设备初始化入口
 */
static int __init dev_init(void){
    filter_srcdev_registe(8,16);   //进行bio截获 
    return 0;
}

static void __exit dev_exit(void){
    filter_srcdev_cancel();
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
