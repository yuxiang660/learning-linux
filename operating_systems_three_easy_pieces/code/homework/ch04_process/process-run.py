#! /usr/bin/env python

import sys
import argparse
import random

# process switch behavior
SCHED_SWITCH_ON_IO = 'SWITCH_ON_IO'
SCHED_SWITCH_ON_END = 'SWITCH_ON_END'

# io finished behavior
IO_RUN_LATER = 'IO_RUN_LATER'
IO_RUN_IMMEDIATE = 'IO_RUN_IMMEDIATE'

# process states
STATE_RUNNING = 'RUNNING'
STATE_READY = 'READY'
STATE_DONE = 'DONE'
STATE_WAIT = 'WAITING'

# members of process structure
PROC_CODE = 'code_'
PROC_PC = 'pc_'
PROC_ID = 'pid_'
PROC_STATE = 'proc_state_'

# things a process can do
DO_COMPUTE = 'cpu'
DO_IO = 'io'

class ProcManager:
    def __init__(self, seed, process_list):
        random.seed(seed)

        self._proc_info = {}
        # example process description (10:100,10:100)
        for p in process_list.split(','):
            self._parse_proc(p)

    def move_to_ready(self, pid, curr_state):
        assert(self._proc_info[pid][PROC_STATE] == curr_state)
        self._proc_info[pid][PROC_STATE] = STATE_READY

    def move_to_wait(self, pid, curr_state):
        assert(self._proc_info[pid][PROC_STATE] == curr_state)
        self._proc_info[pid][PROC_STATE] = STATE_WAIT

    def move_to_running(self, pid, curr_state):
        assert(self._proc_info[pid][PROC_STATE] == curr_state)
        self._proc_info[pid][PROC_STATE] = STATE_RUNNING

    def move_to_done(self, pid, curr_state):
        assert(self._proc_info[pid][PROC_STATE] == curr_state)
        self._proc_info[pid][PROC_STATE] = STATE_DONE

    def get_proc_state(self, pid):
        return self._proc_info[pid][PROC_STATE]

    def get_all_pids(self):
        return list(self._proc_info.keys())

    def get_num_instructions(self, pid):
        return len(self._proc_info[pid][PROC_CODE])

    def pop_next_instruction(self, pid):
        if len(self._proc_info[pid][PROC_CODE]) == 0: return ""
        return self._proc_info[pid][PROC_CODE].pop(0)

    def print_behavior(self, process_switch_behavior, io_done_behavior):
        print('Produce a trace of what would happen when you run these processes:')
        for pid in self.get_all_pids():
            print('Process %d' % pid)
            for inst in range(self.get_num_instructions(pid)):
                print('  %s' % self._proc_info[pid][PROC_CODE][inst])
            print('')
        print('Important behaviors:')
        print('  System will switch when', end=" ")
        if process_switch_behavior == SCHED_SWITCH_ON_IO:
            print('the current process is FINISHED or ISSUES AN IO')
        else:
            print('the current process is FINISHED')
        print('  After IOs, the process issuing the IO will', end=" ")
        if io_done_behavior == IO_RUN_IMMEDIATE:
            print('run IMMEDIATELY')
        else:
            print('run LATER (when it is its turn)')
        print('')

    def _parse_proc(self, process):
        proc_id = self._new_process()
        tmp = process.split(':')
        if len(tmp) != 2:
            print('Bad description (%s): Must be number <x:y>' % process)
            print('  where X is the number of instructions')
            print('  and Y is the percent change that an instruction is CPU not IO')
            exit(1)

        num_instructions, chance_cpu = int(tmp[0]), float(tmp[1])/100.0
        for _ in range(num_instructions):
            if random.random() < chance_cpu:
                self._proc_info[proc_id][PROC_CODE].append(DO_COMPUTE)
            else:
                self._proc_info[proc_id][PROC_CODE].append(DO_IO)

    def _new_process(self):
        proc_id = len(self._proc_info)
        self._proc_info[proc_id] = {}
        self._proc_info[proc_id][PROC_PC] = 0
        self._proc_info[proc_id][PROC_ID] = proc_id
        self._proc_info[proc_id][PROC_CODE] = []
        self._proc_info[proc_id][PROC_STATE] = STATE_READY
        return proc_id


class Scheduler:
    def __init__(self, proc_manager):
        self._proc_manager = proc_manager
        self._clean()

    def _clean(self):
        self.curr_proc = 0
        self.io_finish_times = {}
        for pid in self._proc_manager.get_all_pids():
            self.io_finish_times[pid] = []

    def run(self, process_switch_behavior, io_done_behavior, io_length):
        self._clean()
        clock_tick = 0
        io_busy = 0
        cpu_busy = 0
        if len(self._proc_manager.get_all_pids()) == 0: return (cpu_busy, io_busy, clock_tick)

        self._print_header()

        self._proc_manager.move_to_running(self.curr_proc, STATE_READY)
        while self._has_active_proc():
            clock_tick += 1

            io_done = self._handle_io_done_proc(clock_tick, io_done_behavior)

            next_instruction = ""
            if self._proc_manager.get_proc_state(self.curr_proc) == STATE_RUNNING:
                next_instruction = self._proc_manager.pop_next_instruction(self.curr_proc)
                cpu_busy += 1
            if self._get_ios_in_flight(clock_tick) > 0: io_busy += 1

            self._print_instruction(clock_tick, io_done, next_instruction)

            if next_instruction == DO_IO:
                self._proc_manager.move_to_wait(self.curr_proc, STATE_RUNNING)
                self.io_finish_times[self.curr_proc].append(clock_tick + io_length)
                if process_switch_behavior == SCHED_SWITCH_ON_IO: self._next_proc()

            self._finish_proc_if_done()

        return (cpu_busy, io_busy, clock_tick)


    def _handle_io_done_proc(self, clock_tick, io_done_behavior):
        io_done = False
        for pid in self._proc_manager.get_all_pids():
            if clock_tick in self.io_finish_times[pid]:
                io_done = True
                self._proc_manager.move_to_ready(pid, STATE_WAIT)
                if io_done_behavior == IO_RUN_IMMEDIATE:
                    if self.curr_proc != pid:
                        if self._proc_manager.get_proc_state(self.curr_proc) == STATE_RUNNING:
                            self._proc_manager.move_to_ready(self.curr_proc, STATE_RUNNING)
                    self.curr_proc = pid
                    self._proc_manager.move_to_running(self.curr_proc, STATE_READY)
                else:
                    assert(io_done_behavior == IO_RUN_LATER)
                    if self._get_num_runnable() == 1:
                        # only switch to io done process when there is no other runnable prcess
                        self.curr_proc = pid
                        self._proc_manager.move_to_running(self.curr_proc, STATE_READY)
                self._finish_proc_if_done()
        return io_done

    def _print_header(self):
        print('%s' % 'Time', end=" ")
        for pid in self._proc_manager.get_all_pids():
            print('%10s' % ('PID:%2d' % (pid)), end=" ")
        print('%10s' % 'CPU', end=" ")
        print('%10s' % 'IOs', end=" ")
        print('')

    def _print_instruction(self, clock_tick, io_done, next_instruction):
        if io_done:
            print('%3d*' % clock_tick, end=" ")
        else:
            print('%3d ' % clock_tick, end=" ")
        for pid in self._proc_manager.get_all_pids():
            if pid == self.curr_proc and next_instruction != '':
                print('%10s' % ('RUN:'+next_instruction), end=" ")
            else:
                print('%10s' % (self._proc_manager.get_proc_state(pid)), end=" ")
        if next_instruction == '':
            print('%10s' % ' ', end=" ")
        else:
            print('%10s' % 1, end=" ")
        num_outstanding = self._get_ios_in_flight(clock_tick)
        if num_outstanding > 0:
            print('%10s' % str(num_outstanding), end=" ")
        else:
            print('%10s' % ' ', end=" ")
        print('')

    def _next_proc(self):
        pid_list = self._proc_manager.get_all_pids()
        curr_index = pid_list.index(self.curr_proc)
        next_pid_list = pid_list[curr_index+1:] + pid_list[0:curr_index]
        for pid in next_pid_list:
            if self._proc_manager.get_proc_state(pid) == STATE_READY:
                self.curr_proc = pid
                self._proc_manager.move_to_running(self.curr_proc, STATE_READY)

    def _finish_proc_if_done(self):
        if self._proc_manager.get_num_instructions(self.curr_proc) == 0 and self._proc_manager.get_proc_state(self.curr_proc) == STATE_RUNNING:
            self._proc_manager.move_to_done(self.curr_proc, STATE_RUNNING)
            self._next_proc()

    def _has_active_proc(self):
        num_active = 0
        for pid in self._proc_manager.get_all_pids():
            if self._proc_manager.get_proc_state(pid) != STATE_DONE:
                num_active += 1
        return num_active > 0

    def _get_num_runnable(self):
        num_active = 0
        for pid in self._proc_manager.get_all_pids():
            state = self._proc_manager.get_proc_state(pid)
            if state == STATE_READY or state == STATE_RUNNING:
                num_active += 1
        return num_active

    def _get_ios_in_flight(self, current_time):
        num_in_flight = 0
        for pid in self._proc_manager.get_all_pids():
            for t in self.io_finish_times[pid]:
                if t > current_time:
                    num_in_flight += 1
        return num_in_flight


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Schedule Processes')

    parser.add_argument('-s', '--seed', dest='seed', type=int, default=0, help='the random seed')
    parser.add_argument('-l', '--processlist', dest='process_list',
                        help='a comma-separated list of processes to run, in the form X1:Y1,X2:Y2,... where X is the number of instructions that process should run, and Y the chances (from 0 to 100) that an instruction will use the CPU or issue an IO')
    parser.add_argument('-L', '--iolength', dest='io_length', type=int, default=5, help='how long an IO takes')
    parser.add_argument('-S', '--switch', dest='process_switch_behavior', default='SWITCH_ON_IO', choices=['SWITCH_ON_IO', 'SWITCH_ON_END'],
                        help='when to switch between processes: SWITCH_ON_IO, SWITCH_ON_END')
    parser.add_argument('-I', '--iodone', dest='io_done_behavior', default='IO_RUN_LATER', choices=['IO_RUN_LATER', 'IO_RUN_IMMEDIATE'],
                        help='type of behavior when IO ends: IO_RUN_LATER, IO_RUN_IMMEDIATE')

    options = parser.parse_args()

    proc_manager = ProcManager(options.seed, options.process_list)
    proc_manager.print_behavior(options.process_switch_behavior, options.io_done_behavior)

    scheduler = Scheduler(proc_manager)
    (cpu_busy, io_busy, clock_tick) = scheduler.run(options.process_switch_behavior, options.io_done_behavior, options.io_length)

    print('')
    print('Stats: Total Time %d' % clock_tick)
    print('Stats: CPU Busy %d (%.2f%%)' % (cpu_busy, 100.0 * float(cpu_busy)/clock_tick))
    print('Stats: IO Busy  %d (%.2f%%)' % (io_busy, 100.0 * float(io_busy)/clock_tick))
    print('')
