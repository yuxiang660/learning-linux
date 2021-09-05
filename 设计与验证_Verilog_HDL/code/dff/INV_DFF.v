`timescale 1ns/100ps

module INV_DFF (Clock, Reset_n, DataIn, DataOut);

input Clock;
input Reset_n;
input DataIn;
output DataOut;

reg DataOut;
wire DataInv;

always @(posedge Clock or negedge Reset_n)
begin
    if (~Reset_n)
        DataOut <= 1'b0;
    else
        DataOut <= DataInv;
end

assign #3 DataInv = ~DataIn;

endmodule
