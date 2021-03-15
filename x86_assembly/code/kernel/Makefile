TARGETS = mbr.bin core.bin prog.bin

all: $(TARGETS)

%.bin: %.asm
	nasm $< -o $@

clean:
	@rm -rf *.bin
