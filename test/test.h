#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/bitmap.h>
#include <linux/bitops.h>
#include <linux/types.h>
#include <asm/bitops.h>

#define BITMAP_BIT 4096
#define BITMAP_BYTE (BITMAP_BIT/8)
#define PART(nr) ((nr)/BITMAP_BIT)
#define POSITION(nr) ((nr)%BITMAP_BIT)
