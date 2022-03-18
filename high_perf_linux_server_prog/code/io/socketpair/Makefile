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

# ll -h /proc/`pidof main`/fd
#  lrwx------ 1 yuxiangw yuxiangw 64 3月  18 18:57 3 -> 'socket:[7540742]'
#  lrwx------ 1 yuxiangw yuxiangw 64 3月  18 18:57 4 -> 'socket:[7540743]'
# cat /proc/net/unix | grep 7540742
#  Num       RefCount Protocol Flags    Type St Inode Path
#  0000000000000000: 00000003 00000000 00000000 0001 03 7540742
debug:
	gdb -q ./main
