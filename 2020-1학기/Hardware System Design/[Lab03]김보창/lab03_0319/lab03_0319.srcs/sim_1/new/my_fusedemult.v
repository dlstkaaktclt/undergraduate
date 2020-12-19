`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/03/26 19:32:35
// Design Name: 
// Module Name: my_fusedemult
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module my_fusedmult #(parameter BITWIDTH = 32)
(
input [BITWIDTH-1:0] ain,
input [BITWIDTH-1:0] bin,
input en,
input clk,
output [2*BITWIDTH-1:0] dout
);
/* IMPLEMENT HERE! */

reg [2*BITWIDTH-1:0] result;
//need register because of need of saving last value

assign dout = result;
// wireing register to output


initial begin
    result = 0;
end
// initialize register

always @(posedge clk) begin
    if (en) begin
         result = result + ain * bin;
    end
    else begin
         result = 0;
    end
end





endmodule
