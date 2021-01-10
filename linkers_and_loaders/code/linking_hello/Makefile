all:main

main:
	gcc a.c m.c -o main -m32

%.s : %.c
	gcc -S $< -o $@ -m32

%.o : %.c
	gcc -c $< -o $@ -m32

clean:
	@rm -rf *.o main

dump_m: m.o
	objdump -x m.o

disa_m: m.o
	objdump -d m.o

sym_m: m.o
	objdump -t m.o
