`timescale 1ns/1ns

module tb;

reg X, Y, C_in;
wire SUM, C_out;

initial begin
   X <= 0;
   Y <= 0;
   C_in <= 0;
   #2

   X <= 1;
   Y <= 1;
   C_in <= 1;
   #2

   X <= 1;
   Y <= 0;
   C_in <= 0;
   #2

   X <= 1;
   Y <= 1;
   C_in <= 0;
   #2

   $finish;
end

initial
  begin
    $dumpfile("main.vcd");
    $dumpvars(0, tb);
  end

FullAdd f0(X, Y, C_in, SUM, C_out);

endmodule
