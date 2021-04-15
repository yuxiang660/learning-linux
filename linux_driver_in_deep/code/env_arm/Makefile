all: run

run:
	qemu-system-arm -nographic -sd vexpress.img -M vexpress-a9 -m 512M -kernel zImage -dtb vexpress-v2p-ca9.dtb  -smp 4 -append "init=/linuxrc root=/dev/mmcblk0p1 rw rootwait earlyprintk console=ttyAMA0" 2>/dev/null

run_lcd:
	qemu-system-arm -sd vexpress.img -M vexpress-a9 -m 512M -kernel zImage -dtb vexpress-v2p-ca9.dtb  -append "init=/linuxrc root=/dev/mmcblk0p1 rw rootwait" -net nic -net user

IMAGE_PATH := ../0_arm_kernel/extra

copy:
	cp ${IMAGE_PATH}/zImage . && cp ${IMAGE_PATH}/vexpress.img .
