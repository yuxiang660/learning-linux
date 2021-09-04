all: compile run

SRCS_V=$(wildcard ./*.v)
WAVE_VCD=$(wildcard ./*.vcd)

compile: $(SRCS_V)
	iverilog -o main $^

# generate main.vcd if the design dump vcd file
run:
	vvp main

show: $(WAVE_VCD)
	gtkwave $^

clean:
	@rm -rf *.out *.vcd main

.PHONY: compile run clean show

