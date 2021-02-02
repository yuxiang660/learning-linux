all: target

target:
	gcc hello.c -o main

pre_compile:
	gcc -E hello.c -o hello.i

compile: pre_compile
	gcc -S hello.i -o hello.s

as: compile
	gcc -c hello.s -o hello.o

ld: as
	ld -static /usr/lib/x86_64-linux-gnu/crt1.o /usr/lib/x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/9/crtbeginT.o \
	-L/usr/lib/gcc/x86_64-linux-gnu/9 -L/usr/lib -L/lib hello.o --start-group -lgcc -lgcc_eh -lc --end-group /usr/lib/gcc/x86_64-linux-gnu/9/crtend.o /usr/lib/x86_64-linux-gnu/crtn.o \
	-o main

clean:
	@rm -rf main *.out *.o *.s *.i