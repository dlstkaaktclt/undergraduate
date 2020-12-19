`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/03/24 17:09:16
// Design Name: 
// Module Name: my_add
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

module my_add #(parameter BITWIDTH = 32)
(
input [BITWIDTH-1:0] ain,
input [BITWIDTH-1:0] bin,
output [BITWIDTH-1:0] dout,
output overflow
);
/* IMPLEMENT HERE! */

assign {overflow, dout} = ain + bin;

endmodule