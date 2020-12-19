`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/04/03 19:16:07
// Design Name: 
// Module Name: adder_array
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


module adder_array(cmd, ain0, ain1, ain2, ain3, bin0, bin1, bin2, bin3, dout0, dout1, dout2, dout3, overflow);


input [2:0] cmd;
input [31:0] ain0, ain1, ain2, ain3;
input [31:0] bin0, bin1, bin2, bin3;
output [31:0] dout0, dout1, dout2, dout3;
output [3:0] overflow;


wire [31:0] ain [3:0];
wire [31:0] bin [3:0];
wire [31:0] dout [3:0];
wire temp_overflow[3:0];

assign {ain[0], ain[1], ain[2], ain[3]} = {ain0, ain1, ain2, ain3};
assign {bin[0], bin[1], bin[2], bin[3]} = {bin0, bin1, bin2, bin3};

genvar i;

generate for(i=0; i<4; i=i+1) begin:adder
    my_add #(32) MY_ADDER(
       .ain(ain[i]),
       .bin(bin[i]),
       .dout(dout[i]),
       .overflow(temp_overflow[i])
    );
end endgenerate

assign dout0 = (cmd == 3'b000) ? dout[0] :
               (cmd == 3'b100) ? dout[0] :
               {(32){1'b0}};
               
assign dout1 = (cmd == 3'b001) ? dout[1] :
               (cmd == 3'b100) ? dout[1] :
               {(32){1'b0}};

assign dout2 = (cmd == 3'b010) ? dout[2] :
               (cmd == 3'b100) ? dout[2] :
               {(32){1'b0}};
              
assign dout3 = (cmd == 3'b011) ? dout[3] :
               (cmd == 3'b100) ? dout[3] :
               {(32){1'b0}};

assign overflow = (cmd == 3'b000) ? {1'b0, 1'b0, 1'b0, temp_overflow[0]} :
                  (cmd == 3'b001) ? {1'b0, 1'b0,  temp_overflow[1], 1'b0} :
                  (cmd == 3'b010) ? {1'b0, temp_overflow[2], 1'b0, 1'b0} :
                  (cmd == 3'b011) ? {temp_overflow[3], 1'b0, 1'b0, 1'b0} :
                  (cmd == 3'b100) ? {temp_overflow[3], temp_overflow[2], temp_overflow[1], temp_overflow[0]} :
                  {(4){1'b0}};
        
endmodule
