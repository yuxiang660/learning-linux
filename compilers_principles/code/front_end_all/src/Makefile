export CLASSPATH = $(CURDIR)

all: build

build: */*.java
	javac $^

# Copy the contents in ../test/test_.txt after run
# Then enter following commands in the console: "enter", "ls", "enter"
# The outputs will be printed at the console
# Refer to: https://blog.csdn.net/Zheng_lan/article/details/115916527
run:
	java main.Main

clean:
	@rm -f */*.class
