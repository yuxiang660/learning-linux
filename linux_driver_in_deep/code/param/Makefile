MODULE_NAME := book

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
	sudo insmod book.ko book_name='good_book' book_num=1000

uninstall:
	sudo rmmod book



