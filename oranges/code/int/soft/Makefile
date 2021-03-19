all: com

# Run at dos, the start adress is changed from 0x7c00 to 0x0100
# https://forum.nasm.us/index.php?topic=1297.0
# linux> sudo apt-get install dosbox
# linux> dosbox
# dos> mount c /home/ben
# dos> c:
# dos> cd "pmtest.com folder"
# dos> pmtest.com
# Show "P" at the screen
com:
	nasm pmtest.asm -o pmtest.com

bochs:
	bochs -f ./bochsrc

# Use bximage to create floppy bochs image "pm.img"
copy:
	sudo mount -o loop pm.img /mnt
	sudo cp pmtest.com /mnt
	sudo umount /mnt
