`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/04/03 20:34:57
// Design Name: 
// Module Name: tb_adder_array
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


module tb_adder_array();

    reg [2:0] cmd;
    reg [32-1:0] ain[3:0];
    reg [32-1:0] bin[3:0];

    wire [32-1:0] dout0, dout1, dout2, dout3;
    wire [3:0] overflow;
    
    //for test
    integer i,j,k;
    //random test vector generation
    initial begin
        for(i=0; i<8; i=i+1) begin
            cmd = i;
            for(j=0; j<4; j=j+1) begin
                for(k=0; k<4; k=k+1) begin
                    ain[k] = $urandom%(2**31);
                    bin[k] = $urandom%(2**31);
                end
                #20;
            end  // for each cmd, make random input
        end 
        
        #100;
        cmd = 4;
        ain[0] = {(32){1'b1}};
        ain[1] = {(32){1'b0}};
        ain[2] = {(32){1'b1}};
        ain[3] = {(32){1'b0}};
        bin[0] = {(32){1'b1}};
        bin[1] = {(32){1'b1}};
        bin[2] = {(32){1'b1}};
        bin[3] = {(32){1'b1}};
        
    end
    
    adder_array ADDARRAY(
        .cmd(cmd),
        .ain0(ain[0]),
        .ain1(ain[1]),
        .ain2(ain[2]),
        .ain3(ain[3]),
        .bin0(bin[0]),
        .bin1(bin[1]),
        .bin2(bin[2]),
        .bin3(bin[3]),
        .dout0(dout0),
        .dout1(dout1),
        .dout2(dout2),
        .dout3(dout3),
        .overflow(overflow)
    );
    
    
endmodule
