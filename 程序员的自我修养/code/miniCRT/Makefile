all: main

minicrt:
	gcc -m32 -c -fno-builtin -nostdlib -fno-stack-protector entry.c malloc.c stdio.c string.c printf.c
	ar -rs minicrt.a malloc.o printf.o stdio.o string.o

test:
	gcc -m32 -c -ggdb -fno-builtin -nostdlib -fno-stack-protector test.c

main: minicrt test
	ld -m elf_i386 -static -e mini_crt_entry entry.o test.o minicrt.a -o main

clean:
	@rm -rf *.o minicrt.a main
