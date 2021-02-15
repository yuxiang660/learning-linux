all: thread

thread: thread.c common.h
	gcc -o main thread.c -Wall -pthread

run:
	./main 100000

stop:
	@pkill main

clean:
	@rm -rf main
