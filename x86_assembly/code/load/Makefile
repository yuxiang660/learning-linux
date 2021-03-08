MBR = mbr
PRO = program

all: mbr pro

mbr: $(MBR).asm
	nasm $< -o $(MBR).bin

pro: $(PRO).asm
	nasm $< -o $(PRO).bin

clean:
	@rm -rf *.bin
