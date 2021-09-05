`timescale 1ns/100ps

module TB ;
reg Ck, Rst_n, Din;
wire Dout;

//Clock generation
initial
begin
    Ck = 0;
    forever 
        Ck = #10 ~ Ck;
end

//Reset generation
initial
begin
    Rst_n = 1;
    #5;
    Rst_n = 0;
    #55;
    Rst_n = 1;
end

//Data Input Generation
initial
begin
    Din = 0;
    #80;
    Din = 1;
    #40;
    Din = 0;
    #40;
    $finish;
end

initial
  begin
    $dumpfile("main.vcd");
    $dumpvars(0, TB);
  end

INV_DFF u_INV_DFF (
    .Clock   (Ck),
    .Reset_n (Rst_n),
    .DataIn  (Din),
    .DataOut (Dout)
);
endmodule
