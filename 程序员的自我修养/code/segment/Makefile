all: target

target:
	gcc -static SectionMapping.c -o SectionMapping.elf

sec:
	readelf -S SectionMapping.elf

seg:
	readelf -l SectionMapping.elf
