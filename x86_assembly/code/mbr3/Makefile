TARGET = mbr

all: img

bin: $(TARGET).asm
	nasm $< -o $(TARGET).bin

lst: bin
	ndisasm -o 0x7c00 $(TARGET).bin > $(TARGET).lst

img: bin
	dd if=$(TARGET).bin of=$(TARGET).img bs=512 count=1 conv=notrunc

# If bochs enables debug mode, enter "c" after running, otherwise it is always black.
bochs:
	bochs -f ./bochsrc

# -fda/-fdb file  use 'file' as floppy disk 0/1 image
qemu:
	qemu-system-i386 -fda ./mbr.img

clean:
	@rm -rf *.img *.bin *.lst
