#include<linux/init.h>
#include<linux/module.h>
#include<linux/genhd.h>
#include<linux/bio.h>
#include<linux/blk_types.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/slab.h>
#include<linux/blkdev.h>
#include<linux/miscdevice.h>
#include<linux/string.h>
#include<linux/types.h>
#include<linux/mutex.h>
#include<linux/wait.h>
#include<linux/device.h>
#include<asm/fcntl.h>
#include<linux/atomic.h>
#include<linux/kdev_t.h>
#include<linux/bitops.h>
#include<asm/bitops.h>
#include<linux/bitmap.h>

