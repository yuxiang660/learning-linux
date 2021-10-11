all: target

obj:
	g++ -c main.cpp -o main.o

target: obj
	g++ main.o -o main

clean:
	@rm -rf main *.o
