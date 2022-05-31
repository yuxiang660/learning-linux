MODULE_NAME := hello

# Kernel modules
obj-m += $(MODULE_NAME).o

# Specify flags for the module compilation.
#EXTRA_CFLAGS=-g -O0

build:
	make -C /lib/modules/`uname -r`/build M=$(PWD) modules

install:
	sudo insmod $(MODULE_NAME).ko

uninstall:
	sudo rmmod $(MODULE_NAME)

show:
	ls /sys/module/$(MODULE_NAME)

clean:
	make -C /lib/modules/`uname -r`/build M=$(PWD) clean
