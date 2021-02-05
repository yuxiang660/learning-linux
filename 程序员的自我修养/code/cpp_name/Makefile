all: target

target:
	g++ -c name.cpp -o name.o

symbol: target
	nm name.o
	readelf -s name.o

clean:
	@rm -rf *.o