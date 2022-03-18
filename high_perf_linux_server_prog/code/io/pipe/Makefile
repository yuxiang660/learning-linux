CC=g++
CFLAGS=-g3 -O0 -fno-inline
LDFLAGS=

SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)

.PHONY: clean main

all: main

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

main: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

clean:
	@rm -rf main *.o

# ll -h /proc/<PID>/fd
#  lr-x------ 1 yuxiangw yuxiangw 64 3月  18 18:54 3 -> 'pipe:[7535703]'
debug:
	gdb -q ./main
