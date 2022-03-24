CC=g++
CFLAGS=-g3 -O0 -fno-inline
LDFLAGS=

SRCS=server.cpp
OBJS=$(SRCS:.cpp=.o)

.PHONY: clean server

all: clean server

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

server: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

clean:
	@rm -rf server *.o

# kill -SIGHUP `pidof server`
# kill -SIGCHLD `pidof server`
# kill -SIGTERM `pidof server`
# kill -SIGINT `pidof server`
run:
	./server 127.0.0.1 12345

