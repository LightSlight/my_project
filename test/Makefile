ifneq ($(KERNELRELEASE),)
	obj-m := test.o
else
	PWD := $(shell pwd)
	KDIR := /lib/modules/$(shell uname -r)/build
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
.PHONY:
	all clean
endif
