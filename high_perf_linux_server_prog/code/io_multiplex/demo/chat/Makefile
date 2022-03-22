CC=g++
CFLAGS=-g3 -O0 -fno-inline
LDFLAGS=-pthread

SRCS=server.cpp
OBJS=$(SRCS:.cpp=.o)

SRCS_2=client.cpp
OBJS_2=$(SRCS_2:.cpp=.o)

.PHONY: clean server client

all: clean server client

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

server: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

client: $(OBJS_2)
	$(CC) $(LDFLAGS) $(OBJS_2) -o $@

clean:
	@rm -rf server client *.o

