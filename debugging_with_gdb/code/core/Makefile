all: target

target:
	g++ -g main.cpp -o main

enable_core:
	ulimit -c unlimited
	sudo systemctl enable apport.service

clean:
	@rm -rf main core
