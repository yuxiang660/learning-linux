CC=g++
CFLAGS=-g3 -O0 -fno-inline
LDFLAGS=-pthread

SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)

.PHONY: clean main

all: clean main

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

# kill -SIGUSR1 `pidof main`
# kill -SIGQUIT `pidof main`
main: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

clean:
	@rm -rf main *.o *.log
