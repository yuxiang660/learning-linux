all: help

help:
	./process-run.py -h

# https://zhanghuimeng.github.io/post/ostep-ch-04-homework-simulation-process-run-py/

run_comp:
	./process-run.py -l 2:100,2:100

run_comp_io:
	./process-run.py -l 2:100,1:0

run_io_comp:
	./process-run.py -l 1:0,2:100

run_io_comp_switch_on_end:
	./process-run.py -l 1:0,2:100 -S SWITCH_ON_END

run_io_immediate:
	./process-run.py -l 1:0,5:100
	./process-run.py -l 1:0,5:100 -I IO_RUN_IMMEDIATE

