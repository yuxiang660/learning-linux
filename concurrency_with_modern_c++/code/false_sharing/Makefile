.PHONY: clean main no_false

all: clean main

main:
	g++ main.cpp -pthread -o main

no_false:
	g++ -DNOFALSE main.cpp -pthread -o main

clean:
	@rm -rf main *.o
