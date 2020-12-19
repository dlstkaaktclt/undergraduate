`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/04/03 15:01:00
// Design Name: 
// Module Name: tb_lab4
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


module tb_lab4_ipadd();
    
    reg [32-1:0] ain;
    reg [32-1:0] bin;
    reg [32-1:0] cin;
    reg clk;
    wire [64-1:0] res;
    wire [48-1:0] dummy;
    
    //for test
    integer i;
    //random test vector generation
    initial begin
        clk<=0;
        for(i=0; i<32; i=i+1) begin
            ain = $urandom%(2**31);
            bin = $urandom%(2**31);
            cin = $urandom%(2**31);
            #20;
        end
    end 
    
    always #5 clk = ~clk;
    
    xbip_multadd_0 ADDER(
        .CLK(clk),
        .CE(1'b1),
        .SCLR(1'b0),
        .A(ain),
        .B(bin),
        .C(cin),
        .SUBTRACT(1'b0),
        .P(res),
        .PCOUT(dummy)
    );

endmodule
