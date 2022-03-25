CC=g++
CFLAGS=-g3 -O0 -fno-inline -I/usr/include
LDFLAGS=-L/usr/lib/x86_64-linux-gnu/ -levent

SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)

.PHONY: clean main

all: clean main

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

main: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

clean:
	@rm -rf main *.o *.log
