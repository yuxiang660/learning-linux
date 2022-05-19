CC=g++
CFLAGS=-g3 -O0 -fno-inline
LDFLAGS=-pthread

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

run:
	@./run.sh 2000

debug:
	gdb -q ./main \
	-ex "run" \
	-ex "bt"


