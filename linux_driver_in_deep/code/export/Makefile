MODULE_NAME := export_symb

# Kernel modules
obj-m += $(MODULE_NAME).o

# Specify flags for the module compilation.
#EXTRA_CFLAGS=-g -O0

build: kernel_modules

kernel_modules:
	make -C /lib/modules/`uname -r`/build M=$(PWD) modules

clean:
	make -C /lib/modules/`uname -r`/build M=$(PWD) clean

install:
	sudo insmod $(MODULE_NAME).ko

uninstall:
	sudo rmmod $(MODULE_NAME)

show:
	cat /proc/kallsyms | grep "add_integar\|sub_integar"



