`timescale 1ns/1ns

module tb;
reg val1;
reg val2;

initial
  begin
    $display ("tb1: `timescale 1ns/1ns");

    val1 <= 1;
    val2 <= 2;
    $display ("val1 = %d", val1);
    $display ("val2 = %d", val2);

    $display ("val2 = val1");
    val2 = val1;
    $display ("val1 = %d", val1);
    $display ("val2 = %d", val2);

    #1 $display ("T=%0t At time #1", $realtime);

    $display ("val1 = %d", val1);
    $display ("val2 = %d", val2);

    #1 $display ("T=%0t At time #1", $realtime);
    $display ("End");
  end

initial
  begin
    $dumpfile("main.vcd");
    //数0表示转储模块下面各个层的所有信号。如果是数字1，则表示只转储此模块中的变量，不转储此模块调用模块中的变量
    $dumpvars(0, tb);
  end

endmodule
