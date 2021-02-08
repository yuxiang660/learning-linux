all: target

target:
	gcc -c -fno-builtin -m32 TinyHelloWorld.c
	ld -static -m elf_i386 -e nomain -o main TinyHelloWorld.o

tiny:
	gcc -c -fno-builtin -m32 TinyHelloWorld.c
	ld -static -T TinyHelloWorld.lds -m elf_i386 -e nomain -o main_tiny TinyHelloWorld.o

strip: tiny
	strip -s main_tiny -o main_strip

sec:
	readelf -S main
	readelf -S main_tiny
	readelf -S main_strip

clean:
	@rm -rf main *.o main_tiny main_strip