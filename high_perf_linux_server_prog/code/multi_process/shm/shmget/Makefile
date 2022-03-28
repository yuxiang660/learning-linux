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
	$(CC) $(OBJS) $(LDFLAGS) -o $@

clean:
	@rm -rf main *.o *.log

# Check shm state
#  ipcs -m -i <shmid>
#  cat /proc/sysvipc/shm
debug:
	gdb -q ./main \
	-ex "b main.cpp:31" \
	-ex "run" \
	-ex "shell ipcs -m"
