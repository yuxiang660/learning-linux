`timescale 1ns/100ps

module tb;

reg [1:0] A, B;
reg clk, rstn, sel;
wire A_out, B_out;

always #10 clk <= ~clk;

HelloVlog h0 (
   .Clock(clk),
   .Reset_n(rstn),
   .A_in(A),
   .B_in(B),
   .Sel_in(sel),
   .A_xor_out(A_out),
   .B_xor_out(B_out)
);

initial
begin
   // 1. Initialize testbench variables
   clk <= 0;
   rstn <= 0;
   sel <= 0;
   A <= 2'b00;
   B <= 2'b00;

   // 2. Apply reset to the design
   repeat (2) @ (posedge clk);
   rstn <= 1;
   repeat (5) @ (posedge clk);

   // 3. Set inpputs
   sel <= 0;
   A <= 2'b01;
   B <= 2'b10;
   repeat(5) @ (posedge clk);

   sel <= 1;
   A <= 2'b00;
   B <= 2'b11;
   repeat(5) @ (posedge clk);

   rstn <= 1'b0;
   repeat(5) @ (posedge clk);

   $display("End TB");
   $finish;
end

initial
  begin
   $dumpfile("main.vcd");
   //数0表示转储模块下面各个层的所有信号。如果是数字1，则表示只转储此模块中的变量，不转储此模块调用模块中的变量
   $dumpvars(0, tb);
  end

endmodule
