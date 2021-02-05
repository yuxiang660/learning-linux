all: target

target:
	gcc -m32 -c SimpleSection.c

clean:
	@rm -rf *.o

objdump: target
	objdump -h SimpleSection.o

size: target
	size SimpleSection.o

code: target
	objdump -s -d SimpleSection.o

bss: target
	objdump -x SimpleSection.o

head: target
	readelf -h SimpleSection.o

section: target
	readelf -S SimpleSection.o

symbol: target
	nm SimpleSection.o
	readelf -s SimpleSection.o
