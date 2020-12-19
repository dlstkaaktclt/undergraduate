`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/03/26 19:30:41
// Design Name: 
// Module Name: my_mul
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


module my_mul #(parameter BITWIDTH = 32)
(
input [BITWIDTH-1:0] ain,
input [BITWIDTH-1:0] bin,
output [2*BITWIDTH-1:0] dout
);
/* IMPLEMENT HERE! */

assign dout = ain * bin;

endmodule
