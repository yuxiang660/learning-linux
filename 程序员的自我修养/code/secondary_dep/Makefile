fooso:
	gcc -shared -o libfoo.so -fPIC foo.c

bara:
	gcc -c bar.c
	ar rcs libbar.a bar.o

barso:
	gcc -shared -o libbar.so -fPIC bar.c -lfoo -L$(PWD)

bardump:
	gcc -shared -o libbar_dumb.so -fPIC bar.c

app_bara: bara fooso
	gcc -o app_bara main.c libbar.a -L$(PWD) -lfoo
	ldd app_bara

run_bara: app_bara
	LD_LIBRARY_PATH=$(PWD) ./app_bara

clean:
	@rm -f *.so *.a app* *.o
