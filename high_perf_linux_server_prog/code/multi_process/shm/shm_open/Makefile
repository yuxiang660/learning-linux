CC=g++
CFLAGS=-g3 -O0 -fno-inline
LDFLAGS=-lrt

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

# Check memory address
#  cat /proc/<pid>/maps
debug:
	gdb -q ./main \
	-ex "start" \
	-ex "shell cat /proc/`pidof main`/maps"
