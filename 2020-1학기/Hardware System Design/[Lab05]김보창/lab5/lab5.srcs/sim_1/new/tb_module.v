`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/04/11 15:48:53
// Design Name: 
// Module Name: tb_module
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


module tb_module();

reg [32-1:0] ain;
reg [32-1:0] din;
reg [31:0] ain_mem [0:15];
reg [31:0] din_mem [0:15];
reg [6-1:0] addr;
reg we;
reg clk;
reg valid;
reg areset;

wire dvalid;
wire [31:0] dout;
//for test
integer i;
//random test vector generation
    initial begin
        clk<=0;
        addr = 0;    
        valid=0;   //internal MAC sholdn't working
        we=1;      // data goes to register

        din_mem[0] = 32'b00111111100000000000000000000000;  //1
        ain_mem[0] = 32'b00111111100000000000000000000000;
        din_mem[1] = 32'b01000000000000000000000000000000;  //2
        ain_mem[1] = 32'b01000000000000000000000000000000;
        din_mem[2] = 32'b01000000010000000000000000000000;  //3
        ain_mem[2] = 32'b01000000010000000000000000000000;
        din_mem[3] = 32'b01000000100000000000000000000000;  //4
        ain_mem[3] = 32'b01000000100000000000000000000000;
        din_mem[4] = 32'b01000000101000000000000000000000;  //5
        ain_mem[4] = 32'b01000000101000000000000000000000;
        din_mem[5] = 32'b01000000110000000000000000000000;  //6
        ain_mem[5] = 32'b01000000110000000000000000000000;
        din_mem[6] = 32'b01000000111000000000000000000000;  //7
        ain_mem[6] = 32'b01000000111000000000000000000000;
        din_mem[7] = 32'b01000001000000000000000000000000;  //8
        ain_mem[7] = 32'b01000001000000000000000000000000;
        din_mem[8] = 32'b01000001000100000000000000000000;  //9
        ain_mem[8] = 32'b01000001000100000000000000000000;
        din_mem[9] = 32'b01000001001000000000000000000000;  //10
        ain_mem[9] = 32'b01000001001000000000000000000000;
        din_mem[10] = 32'b01000001001100000000000000000000;  //11
        ain_mem[10] = 32'b01000001001100000000000000000000;
        din_mem[11] = 32'b01000001010000000000000000000000;  //12
        ain_mem[11] = 32'b01000001010000000000000000000000;
        din_mem[12] = 32'b01000001010100000000000000000000;  //13
        ain_mem[12] = 32'b01000001010100000000000000000000;
        din_mem[13] = 32'b01000001011000000000000000000000;  //14
        ain_mem[13] = 32'b01000001011000000000000000000000;
        din_mem[14] = 32'b01000001011100000000000000000000;  //15
        ain_mem[14] = 32'b01000001011100000000000000000000;
        din_mem[15] = 32'b01000001100000000000000000000000;  //16
        ain_mem[15] = 32'b01000001100000000000000000000000;
        
        areset <= 0;    //reset
        
        #20;
        
        areset <= 1;
        
        for(i=0; i<16; i=i+1) begin     //random generate
            din = din_mem[i];
            #10;  
            addr = addr + 2'b1;          
        end
        
        #10;
        
        addr = 0;
        we=0;   
        
        for(i=0; i<16; i=i+1) begin     
            ain = ain_mem[i];
            #10;    //load inside register value to b_in, and calcuated d_out value into r_dout. now calcualtaion ready.
            valid = 1;   //now internal mac working
            #10;
            valid = 0; // input inserted, so we should wait
            wait(dvalid == 2'b1)
            #5;     //clock edge sync
            addr = addr + 2'b1;    //address change
        end
        #10;
        addr <= 0; 
    end 

    always #5 clk <= ~clk;


    my_pe #(6) PE (
      .aclk(clk),
      .aresetn(areset),
      .ain(ain),
      .din(din),
      .addr(addr),
      .we(we),
      .valid(valid),
      .dvalid(dvalid),
      .dout(dout)
    );
    

endmodule
