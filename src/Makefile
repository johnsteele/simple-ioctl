
ifneq ($(KERNELRELEASE),)
	obj-m := simple_ioctl.o

else 
	KERNELDIR=/lib/modules/$(shell uname -r)/build
	PWD=$(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif 
