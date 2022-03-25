CC=g++
CFLAGS=-g3 -O0 -fno-inline
LDFLAGS=

SRC_1 = server.cpp
OBJS_1=$(SRC_1:.cpp=.o)

.PHONY: clean server

all: clean server

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

server: $(OBJS_1)
	$(CC) $(LDFLAGS) $(OBJS_1) -o $@

clean:
	@rm -rf server client *.o

# Client
#  telnet 127.0.0.1 12345 // disconnected by server after 15 seconds if no data transfer
run:
	./server 127.0.0.1 12345 50
