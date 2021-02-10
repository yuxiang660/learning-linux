all: target

lib:
	gcc -shared -fPIC lib.c -Xlinker --version-script lib.ver -o lib.so

lib_old:
	gcc -shared -fPIC lib.c -Xlinker --version-script lib_old.ver -o lib.so

lib_new:
	gcc -shared -fPIC lib.c -Xlinker --version-script lib_new.ver -o lib.so

target:
	gcc main.c ./lib.so -o main
