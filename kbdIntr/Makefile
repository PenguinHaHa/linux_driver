ifneq ($(KERNELRELEASE),)
	obj-m := kbdintr.o
	module-objs := kbdintr.o
else 
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

clean:
	rm -rf *.o *.ko *.mod.c *.order *.symvers .tmp_versions .*.cmd
