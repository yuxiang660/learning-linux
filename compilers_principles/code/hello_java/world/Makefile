all: build

build: *.java
	@make -C ./moon
	@javac $^

clean:
	@make -C ./moon clean
	@rm -f *.class
