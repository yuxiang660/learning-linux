all: target debug

target:
	gcc main.c -o main

debug:
	gcc main.c -g -o main_debug

section:
	readelf -S main
	readelf -S main_debug

strip: debug
	strip main_debug
