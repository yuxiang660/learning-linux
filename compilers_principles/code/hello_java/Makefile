all: build

build:
	@make -C ./world
	@make -C ./main

export CLASSPATH = $(shell pwd)

run_hello:
	@java world.HelloWorld

run:
	@java main.Main

clean:
	@make -C ./world clean
	@make -C ./main clean

