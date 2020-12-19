`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/04/10 21:26:03
// Design Name: 
// Module Name: tb_gen_input
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


module tb_gen_input();

integer i; 
reg [31:0] mem [0:8191]; 
 
    initial begin 
    for (i=0; i<8192; i = i+1) begin 
           mem[i] <= i; 
    end 
    #10;
    $writememh("input.txt", mem); 
    end 
            
 
endmodule
