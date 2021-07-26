ifneq ($(KERNELRELEASE),)
	obj-m := dev.o
else
	PWD := $(shell pwd)
	KDIR := /lib/modules/$(shell uname -r)/build
	KBUILD_EXTRA_SYMBOLS += /home/liya/test/Module.symvers
	export KBUILD_EXTRA_SYMBOLS
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
.PHONY:
	all clean
endif
