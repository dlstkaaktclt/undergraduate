`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/03/26 19:54:42
// Design Name: 
// Module Name: tb_mul
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

module tb_mul();
    parameter BITWIDTH = 32;
    
    //for my IP
    reg [BITWIDTH-1:0] ain;
    reg [BITWIDTH-1:0] bin;
    wire [2*BITWIDTH-1:0] dout;
    
    //for test
    integer i;
        // random test vector generation
    
    initial begin
        for(i=0; i<32; i=i+1) begin
            ain = $urandom%(2**31);
            bin = $urandom%(2**31);
            #10;
        end
    end
    
    
    //my IP
    my_mul #(BITWIDTH) MY_MUL(
        .ain(ain),
        .bin(bin),
        .dout(dout)
    );
 
   
endmodule
