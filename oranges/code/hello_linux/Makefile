all: target

target:
	nasm -f elf hello.asm -o hello.o
	ld -m elf_i386 -s hello.o -o main

target_64:
	nasm -felf64 hello.asm -o hello.o
	ld -s hello.o -o main

clean:
	@rm -rf *.o main