all: img

bin: boot.asm
	nasm $< -o boot.bin

dis: bin
	ndisasm -o 0x7c00 boot.bin

img: bin
	dd if=boot.bin of=a.img bs=512 count=1 conv=notrunc

# If bochs enables debug mode, enter "c" after running, otherwise it is always black.
run:
	bochs -f ./bochsrc

clean:
	@rm -rf a.img *.bin
