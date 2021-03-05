all: hello_64

hello_64:
	nasm -felf64 hello.asm && ld hello.o -o main

hello_32:
	nasm -felf32 hello_32.asm && ld -m elf_i386 -s hello_32.o -o main_32

# https://forum.nasm.us/index.php?topic=1297.0
# linux> sudo apt-get install dosbox
# linux> dosbox
# dos> mount c /home/ben
# dos> c:
# dos> cd "main.com folder"
# dos> main
hello_16:
	nasm -f bin -o main.com hello_16.asm

clean:
	@rm -rf main_32 main.com main *.o
