TARGETS = mbr.bin key.bin

all: $(TARGETS)

%.bin: %.asm
	nasm $< -o $@

clean:
	@rm -rf *.bin
