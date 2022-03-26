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
	@rm -rf main *.o *.log

# Check semaphore value:
#  ipcs -s -i <semid>
#  cat /proc/sysvipc/sem
debug:
	gdb -q ./main \
	-ex "start" \
	-ex "shell ipcs -s"
