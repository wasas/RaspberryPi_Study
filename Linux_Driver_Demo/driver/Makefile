ifneq ($(KERNELRELEASE),)
	obj-m := demo.o
else
	KERNELDIR:=/home/linux/
    PWD := $(shell pwd)
    CROSS_COMPILE := arm-linux-gnueabihf-
default:
	$(MAKE) CROSS_COMPILE=$(CROSS_COMPILE) ARCH=arm -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.ko *.mod.c *.order *.symvers	
endif
