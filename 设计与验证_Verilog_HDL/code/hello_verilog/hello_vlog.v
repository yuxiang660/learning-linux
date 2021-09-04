`timescale 1ns/100ps

// 声明部分：module, 端口, 线网和寄存器
module HelloVlog (
   Clock,
   Reset_n,
   A_in,
   B_in,
   Sel_in,
   A_xor_out,
   B_xor_out
);

input Clock;
input Reset_n;
input [1:0] A_in;
input [1:0] B_in;
input Sel_in;
output A_xor_out;
output B_xor_out;

wire A_xor_wire;
wire B_xor_wire;
wire [1:0] result;
reg eq0, eq1, eq2, eq3;
reg A_xor_out;
reg B_xor_out;

// 行为描述
always @(posedge Clock or negedge Reset_n) begin
   if (~Reset_n)
      A_xor_out <= 0;
   else
      A_xor_out <= A_xor_wire;
end

always @(posedge Clock or negedge Reset_n) begin
   if (~Reset_n)
      B_xor_out <= 0;
   else
      B_xor_out <= B_xor_wire;
end

// 数据流描述，#1延时一个时间单位来模拟组合逻辑的延时
assign #1 A_xor_wire = eq0 ^ eq1;

//结构化描述，用xor门原语达到上面一样的效果
xor #1 XOR_B(B_xor_wire, eq2, eq3);

assign #3 result = (Sel_in) ? B_in : A_in;

//行为描述
always @(result) begin
   case (result)
      2'b00: begin
         {eq3, eq2, eq1, eq0} = #2 4'b0001;
         $display("At time %t - ", $time, "eq0 = 1");
      end
      2'b01: begin
         {eq3, eq2, eq1, eq0} = #2 4'b0010;
         $display("At time %t - ", $time, "eq1 = 1");
      end
      2'b10: begin
         {eq3, eq2, eq1, eq0} = #2 4'b0100;
         $display("At time %t - ", $time, "eq2 = 1");
      end
      2'b11: begin
         {eq3, eq2, eq1, eq0} = #2 4'b1000;
         $display("At time %t - ", $time, "eq3 = 1");
      end
      default: ;
   endcase
end

endmodule
