fooso:
	gcc -shared -o libfoo.so -fPIC foo.c

bara:
	gcc -c bar.c
	ar rcs libbar.a bar.o

barso: fooso
	gcc -shared -o libbar.so -fPIC bar.c -lfoo -L$(PWD)

bardumb:
	gcc -shared -o libbar_dumb.so -fPIC bar.c

app_bara: bara fooso
	gcc -o app_bara main.c libbar.a -L$(PWD) -lfoo
	ldd app_bara

run_bara: app_bara
	LD_LIBRARY_PATH=$(PWD) ./app_bara

app_bardumb: bardumb fooso
	gcc -o app_bardumb main.c -L$(PWD) -lbar_dumb -lfoo
	ldd app_bardumb

run_bardumb: app_bardumb
	LD_LIBRARY_PATH=$(PWD) ./app_bardumb

app_bardumb_nofoo: bardumb
	gcc -o app_bardumb_nofoo main.c -L$(PWD) -lbar_dumb -Wl,--allow-shlib-undefined
	ldd app_bardumb_nofoo

run_bardumb_nofoo: app_bardumb_nofoo
	LD_PRELOAD=$(PWD)/libfoo.so LD_LIBRARY_PATH=$(PWD) ./app_bardumb_nofoo

app_barso: fooso barso
	gcc -o app_barso main.c -L$(PWD) -lbar -Wl,-rpath-link=$(PWD)
	ldd app_barso

run_barso: app_barso
	LD_LIBRARY_PATH=$(PWD) ./app_barso

app_rpath: fooso barso
	gcc -o app_rpath main.c -L$(PWD) -lfoo -lbar -Wl,-rpath=$(PWD)
	ldd app_rpath

run_rpath: app_rpath
	LD_LIBRARY_PATH=$(PWD) ./app_rpath

clean:
	@rm -f *.so *.a app* *.o
