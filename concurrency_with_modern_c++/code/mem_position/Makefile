CC=g++
CFLAGS=-g3 -O0 -fno-inline -Wno-overflow -Wno-shift-count-overflow
LDFLAGS=

SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)

.PHONY: clean main debug

all: clean main

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

main: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

clean:
	@rm -rf main *.o

debug:
	gdb -q ./main \
	-ex "run" \
	-ex "bt"


