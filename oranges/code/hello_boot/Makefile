all: img

bin: boot.asm
	nasm $< -o boot.bin

dis: bin
	ndisasm -o 0x7c00 boot.bin

img: bin
	dd if=boot.bin of=a.img bs=512 count=1 conv=notrunc

clean:
	@rm -rf a.img *.bin
