all: cpu

cpu: cpu.c common.h
	gcc -o main cpu.c -Wall

run:
	./main A & ./main B & ./main C & ./main D &

stop:
	@pkill main

clean:
	@rm -rf main
