#######################
# Makefile for foobar #
#######################

# Programs, flags, etc.
ASM		= nasm
CC		= gcc
LD		= ld
ASMFLAGS	= -f elf
CFLAGS		= -c  -m32 
LDFLAGS		= -s  -m elf_i386 

# This Program
BIN		= main
OBJS		= foo.o bar.o

# All Phony Targets
.PHONY : clean $(OBJS)

all : $(BIN)

clean :
	rm -f $(OBJS) $(BIN)

$(BIN) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(BIN) $(OBJS)

foo.o : foo.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

bar.o: bar.c
	$(CC) $(CFLAGS) -o $@ $<
