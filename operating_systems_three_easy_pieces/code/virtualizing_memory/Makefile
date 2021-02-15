all: mem

mem: mem.c common.h
	gcc -fno-stack-protector -z execstack -o main mem.c -Wall

# Disable address space layout randomization, otherwise the pointer address will change in every run
dis_aslr:
	sudo sysctl -w kernel.randomize_va_space=0

en_aslr:
	sudo sysctl -w kernel.randomize_va_space=2

run:
	./main & ./main &

stop:
	pkill main

clean:
	@rm -rf main
