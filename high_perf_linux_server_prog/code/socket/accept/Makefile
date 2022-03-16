CC=g++
CFLAGS=-g3 -O0 -fno-inline
LDFLAGS=

SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)

.PHONY: clean main

all: main

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

main: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

clean:
	@rm -rf main *.o

# telnet 127.0.0.1 12345&
# kill -s SIGTERM `pidof main`
# kill -9 `pidof telnet`
# sudo netstat -ntp | grep 12345
run:
	./main 127.0.0.1 12345
