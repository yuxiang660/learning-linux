.PHONY: clean main no_false

all: clean main

# 优化选项可修复伪共享
main:
	g++ -O0 main.cpp -pthread -o main

no_false:
	g++ -O0 -DNOFALSE main.cpp -pthread -o main

clean:
	@rm -rf main *.o
