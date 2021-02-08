all: target

target:
	gcc -c a.c b.c
	ld -e main -lc a.o b.o -o main

clean:
	@rm -rf *.o main
