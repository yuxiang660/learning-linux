// https://www.cxyzjd.com/article/Reborn_Lee/107888798

`timescale 1ns/1ps

module tb;
reg  a, b, c, q;

initial
  begin
    $monitor("[%0t] a=%0b b=%0b c=%0b q=%0b", $time, a, b, c, q);

    // Initialize all signals to 0 at time 0
    a <= 0;
    b <= 0;
    c <= 0;
    q <= 0;

    // Inter-assignment delay: Wait for #5 time units
    // and then assign a and c to 1. Note that 'a' and 'c'
    // gets updated at the end of current timestep
    #5  a <= 1;
        c <= 1;
        q <= a&c;

    // Intra-assignment delay: First execute the statement
    // then wait for 5 time units and then assign the evaluated
    // value to q
    // q <= #5 a & b | c;

    #1 q <= a&c;

    // Inter-assignment delay: Wait for #5 time units
    // and then assign 'q' with whatever value RHS gets
    // evaluated to
    #5 q <= a & b | c;

    #20;
  end

initial
  begin
    $dumpfile("main.vcd");
    //数0表示转储模块下面各个层的所有信号。如果是数字1，则表示只转储此模块中的变量，不转储此模块调用模块中的变量
    $dumpvars(0, tb);
  end

endmodule
