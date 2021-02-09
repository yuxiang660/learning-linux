all: target

lib:
	gcc -fPIC -shared a1.c -o a1.so
	gcc -fPIC -shared a2.c -o a2.so
	gcc -fPIC -shared b1.c a1.so -o b1.so
	gcc -fPIC -shared b2.c a1.so -o b2.so
	gcc -fPIC -shared c.c -o c.so

target: lib
	gcc main.c b1.so b2.so c.so -o main

run:
	export LD_LIBRARY_PATH=. && ./main

clean:
	@rm -rf *.so main
