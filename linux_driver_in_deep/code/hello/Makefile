KERNEL_PATH := ../1_local_kernel

# Kernel modules
obj-m += hello.o

# Specify flags for the module compilation.
#EXTRA_CFLAGS=-g -O0

build: kernel_modules

kernel_modules:
	make -C /lib/modules/`uname -r`/build M=$(PWD) modules

clean:
	make -C /lib/modules/`uname -r`/build M=$(PWD) clean

local_kernel_modules:
	make -C $(KERNEL_PATH) M=$(CURDIR) modules

local_clean:
	make -C $(KERNEL_PATH) M=$(CURDIR) clean
