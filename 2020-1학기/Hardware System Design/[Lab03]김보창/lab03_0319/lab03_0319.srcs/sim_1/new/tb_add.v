`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/03/26 19:47:48
// Design Name: 
// Module Name: tb_add
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

module tb_add();
    parameter BITWIDTH = 32;
    
    //for my IP
    reg [BITWIDTH-1:0] ain;
    reg [BITWIDTH-1:0] bin;
    wire [BITWIDTH-1:0] dout;
    wire overflow;
    
    //for test
    integer i;
        //random test vector generation
       
    initial begin
        for(i=0; i<32; i=i+1) begin
            ain = $urandom%(2**31);
            bin = $urandom%(2**31);
            #10;
        end
    end
    
    //my IP
    my_add #(BITWIDTH) MY_ADDER(
        .ain(ain),
        .bin(bin),
        .dout(dout),
        .overflow(overflow)
    );
endmodule  