all: target

lib:
	gcc -fPIC -shared -o Lib.so Lib.c
	gcc -shared -o Lib_no_pic.so Lib.c

target: lib
	gcc -o main1 Program1.c ./Lib.so
	gcc -o main2 Program2.c ./Lib.so

clean:
	@rm -rf Lib.so main1 main2