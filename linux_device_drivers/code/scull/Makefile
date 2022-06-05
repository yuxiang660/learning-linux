# Comment/uncomment the following line to disable/enable debugging
# DEBUG = y

# Add your debugging flag (or not) to CFLAGS
ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g -DSCULL_DEBUG # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

# Linux kernel build-in option: https://www.kernel.org/doc/Documentation/kbuild/makefiles.txt
EXTRA_CFLAGS += $(DEBFLAGS)

ifneq ($(KERNELRELEASE),)
# call from kernel build system

scull-objs := main.o pipe.o access.o

obj-m	:= scull.o

else

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.mod modules.order *.symvers

# auto-select major device number
install:
	sudo ./scull_load
	cat /proc/devices | grep scull

# specify major device number 111
install2:
	sudo ./scull_load scull_major=111
	cat /proc/devices | grep scull

uninstall:
	sudo ./scull_unload
	cat /proc/devices | grep scull

init:
	sudo ./scull.init $(option)
