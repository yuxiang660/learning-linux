MODULE_NAME := hello

# if KERNELRELEASE is not defined, we've been called directly from the command line.
# Invoke the kernel build system.
ifeq (${KERNELRELEASE},)
KDIR ?= /lib/modules/`uname -r`/build

# 改变目录到-C选项指定的位置(即内核源代码目录)，其中保存有内核的顶层makefile文件
# M=选项让该makefile在构造modules目标之前返回到模块源代码目录。然后，modules目标
# 指向obj-m变量中设定的模块hello.o
default:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

# 装载时可以传递模块中参数的值
install:
	sudo insmod $(MODULE_NAME).ko

# modprobe会把依赖的模块一起加载，从而解决"unresolved symbols"的错误
# https://stackoverflow.com/questions/27098426/module-not-found-modprobe
install2:
	sudo modprobe $(MODULE_NAME)

uninstall:
	sudo rmmod $(MODULE_NAME)

# lsmod读取/proc/modules虚拟文件来获取当前装载到内核中的所有模块
show:
	lsmod
	ls /sys/module/$(MODULE_NAME)

# Otherwise KERNELRELEASE is defined; we've been invoked from the
# kernel build system and can use its language.
else

obj-m := $(MODULE_NAME).o
# $(MODULE_NAME)-objs := file1.o file2.o

endif
