all: compile run

SRCS_V=$(wildcard ./*.v)

compile: $(SRCS_V)
	iverilog -o main $^

run:
	vvp main

clean:
	@rm -rf *.out main

.PHONY: compile run clean

