// Declare the timescale where time_unit is 1ns
// and time_precision is also 1ns
`timescale 1ns/1ns

module tb;
// To understand the effect of timescale, let us
// drive a signal with some values after some delay
reg val;

initial
  begin
    $display ("tb1: `timescale 1ns/1ns");

    // Initialize the signal to 0 at time 0 units
    val <= 0;

    // Advance by 1 time unit, display a message and toggle val
    #1     $display ("T=%0t At time #1", $realtime);
    val <= 1;

    #1   $display ("T=%0t At time #1", $realtime);
    val <= 0;

    #1   $display ("T=%0t At time #1", $realtime);
    val <= 1;

    #1   $display ("T=%0t At time #1", $realtime);
    val <= 0;

    // Let simulation run for another 5 time units and exit
    #5 $display ("T=%0t End of simulation", $realtime);
  end

initial
  begin
    $dumpfile("wave.vcd");
    //数0表示转储模块下面各个层的所有信号。如果是数字1，则表示只转储此模块中的变量，不转储此模块调用模块中的变量
    $dumpvars(0, tb);
  end

endmodule
