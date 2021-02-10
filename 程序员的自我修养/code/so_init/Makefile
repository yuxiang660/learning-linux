all: target

lib:
	gcc -shared -fPIC -Wl,-soname,libfoo.so.1 -o libfoo.so.1.0.0 foo.c
	ldconfig -n .

target: lib
	gcc -Wl,-rpath=. -o main main.c libfoo.so.1

clean:
	@rm -rf libfoo* main
