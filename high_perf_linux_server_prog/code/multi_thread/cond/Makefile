CC=g++
CFLAGS=-g3 -O0 -fno-inline
LDFLAGS=-pthread

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

# Ctrl+C, check the "__owner"(LWP) of the mutex, use "info threads" to get thread LWP
# p mutex_a
# p mutex_b
debug:
	gdb -q ./main \
	-ex "run"
