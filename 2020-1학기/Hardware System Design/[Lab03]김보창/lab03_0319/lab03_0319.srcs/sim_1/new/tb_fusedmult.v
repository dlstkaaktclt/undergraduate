`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/03/26 19:57:28
// Design Name: 
// Module Name: tb_fusedmult
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
`timescale 1ns / 1ps
module tb_fusedmult();
    parameter BITWIDTH = 32;

    //for my IP
    reg [BITWIDTH-1:0] ain;
    reg [BITWIDTH-1:0] bin;
    reg clk;
    reg en;
    wire [2*BITWIDTH-1:0] dout;
    
    //for test
    integer i;
    
    initial begin
        clk<=0;
        en<=0; // <= <-> assign =..
        #30;
        en<=1;
        for(i=0; i<32; i=i+1) begin
            ain = $urandom%(2**31);
            bin = $urandom%(2**31);
            #10;
        end;
     end;
     
     //my IP
     my_fusedmult #(BITWIDTH) MY_MAC(
        .ain(ain),
        .bin(bin),
        .en(en),
        .clk(clk),
        .dout(dout)
     );
     
     always #5 clk = ~clk;
     
endmodule
