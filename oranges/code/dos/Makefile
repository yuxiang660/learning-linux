all: target

target:
	nasm pmtest1.asm -o pmtest1.com

image:
	dd if=pmtest1.bin of=a.img bs=512 count=1 conv=notrunc

# If bochs enables debug mode, enter "c" after running, otherwise it is always black.
bochs:
	bochs -f ./bochsrc

mount:
	sudo mount -o loop pm.img /mnt/floppy
	sudo cp pmtest1.com /mnt/floppy
	sudo umount /mnt/floppy
