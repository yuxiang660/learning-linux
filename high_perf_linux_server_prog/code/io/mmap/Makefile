CC=g++
CFLAGS=-g3 -O0 -fno-inline
LDFLAGS=

SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)

.PHONY: clean main

all: clean main

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

main: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

clean:
	@rm -rf main *.o out_mmap.log

run:
	./main in_mmap.log out_mmap.log

# cat /proc/`pidof main`/maps
#  7ffff7fc8000-7ffff7fc9000 rw-s 00000000 08:05 3825987 /home/yuxiangw/GitHub/learning-linux/high_perf_linux_server_prog/code/io/mmap/out_mmap.log
#    size = 0x1000 (4096 = 4K), same as "du -h out_mmap.log"
#  7ffff7ffb000-7ffff7ffc000 r--s 00000000 08:05 3825985 /home/yuxiangw/GitHub/learning-linux/high_perf_linux_server_prog/code/io/mmap/in_mmap.log
#    size = 0x1000 (4096 = 4K), same as "du -h in_mmap.log"
debug:
	gdb -q --args ./main in_mmap.log out_mmap.log
