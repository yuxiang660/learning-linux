CC=g++
CFLAGS=-g3 -O0 -fno-inline
LDFLAGS=-pthread

SRCS=server.cpp
OBJS=$(SRCS:.cpp=.o)

.PHONY: clean server

all: clean server

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

server: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

clean:
	@rm -rf server client *.o

# TCP client
#  nc 127.0.0.1 12345
# UDP client
#  nc -u 127.0.0.1 12345
run:
	./server 127.0.0.1 12345

