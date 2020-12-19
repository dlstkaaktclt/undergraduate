`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/04/10 19:55:49
// Design Name: 
// Module Name: tb_pe
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


module tb_bram();
  
  integer i;
  
  reg clk;
  reg [14:0] BRAM_ADDR;
  reg [14:0] BRAM_ADDR_delayed_1; // this is used for delay one clock cycle
  reg [14:0] BRAM_ADDR_delayed_2; // this is used for delay two clock cycle
  reg done;
  wire [31:0] BRAM_RDDATA;
  wire [31:0] BRAM_RDDUMMY; // it should return trash value.
  wire [31:0] BRAM_WRDUMMY;  // there is no use for this.

  initial begin
    clk <= 0;
    BRAM_ADDR <= {(15){1'b0}};
    for(i=0; i<8192;i=i+1) begin
        #10;
        BRAM_ADDR <= BRAM_ADDR + 3'b100;
    end
    #20;
    done <= 1'b1;
  end
  
  always #5 clk <= ~clk;
    
  always @ (posedge clk) begin
     BRAM_ADDR_delayed_1 <= BRAM_ADDR;
     BRAM_ADDR_delayed_2 <= BRAM_ADDR_delayed_1;    //delaying for two clock cycle.
  end
  
  my_bram # (15, "input.txt", "output1.txt") bram1 (
    .BRAM_ADDR(BRAM_ADDR),
    .BRAM_CLK(clk),
    .BRAM_WRDATA(BRAM_WRDUMMY),
    .BRAM_RDDATA(BRAM_RDDATA),
    .BRAM_EN(1'b1),
    .BRAM_RST(1'b0),
    .BRAM_WE(4'b0000),
    .done(done)
  );

 my_bram # (15, "", "output2.txt") bram2 (
    .BRAM_ADDR(BRAM_ADDR_delayed_2),
    .BRAM_CLK(clk),
    .BRAM_WRDATA(BRAM_RDDATA),
    .BRAM_RDDATA(BRAM_RDDUMMY),
    .BRAM_EN(1'b1),
    .BRAM_RST(1'b0),
    .BRAM_WE(4'b1111),
    .done(done)
  );
  

endmodule

