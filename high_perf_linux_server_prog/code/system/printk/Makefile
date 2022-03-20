# https://www.geeksforgeeks.org/linux-kernel-module-programming-hello-world-program/

obj-m = hello.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

run:
	sudo insmod hello.ko
	modinfo hello.ko
	lsmod | grep hello
	sudo rmmod hello.ko
	tail -10 /var/log/kern.log

