TARGETS = mbr.bin rtc.bin

all: $(TARGETS)

%.bin: %.asm
	nasm $< -o $@

clean:
	@rm -rf *.bin
