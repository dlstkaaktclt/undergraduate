`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/04/10 18:21:26
// Design Name: 
// Module Name: my_bram
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


module my_bram#(
    parameter integer BRAM_ADDR_WIDTH = 15, // 4x8192
    parameter INIT_FILE = "input.txt",
    parameter OUT_FILE = "output.txt"
    )(
    input wire [BRAM_ADDR_WIDTH-1:0] BRAM_ADDR,
    input wire BRAM_CLK,
    input wire [31:0] BRAM_WRDATA,
    output reg [31:0] BRAM_RDDATA,
    input wire BRAM_EN,
    input wire BRAM_RST,
    input wire [3:0] BRAM_WE,
    input wire done
    );
    reg [31:0] mem[0:8191];
    wire [BRAM_ADDR_WIDTH-3:0] addr = BRAM_ADDR[BRAM_ADDR_WIDTH-1:2];
    reg [31:0] dout;
    
    // code for reading & writing
    initial begin
        if (INIT_FILE != "") begin
            $readmemh(INIT_FILE,mem);
            // read data from INIT_FILE and store them into mem
        end
    
        wait (done)
        // write data stored in mem into OUT_FILE
        $writememh(OUT_FILE, mem);
    end
    //code for BRAM implementation

    always @ (posedge BRAM_CLK) begin
         if (BRAM_EN) begin
            if(|BRAM_WE) begin // reduction to one bit. use reduction or.
                // do write to BRAM
                mem[addr] <= {(BRAM_WRDATA[8*4-1:8*3] & {(8){BRAM_WE[3]}}),
                              (BRAM_WRDATA[8*3-1:8*2] & {(8){BRAM_WE[2]}}),
                              (BRAM_WRDATA[8*2-1:8*1] & {(8){BRAM_WE[1]}}),
                              (BRAM_WRDATA[8*1-1:8*0] & {(8){BRAM_WE[0]}})};
            end
            else begin  
                // do read from BRAM
                dout <= mem[addr];
            end
         end
    end
    
    always @ (posedge BRAM_CLK) begin   // dout comes to BRAM_RDDATA. it makes read value after two cycle. reseted value also appear two cycle.
       BRAM_RDDATA <= (dout & {(32){!BRAM_RST}});
       /*
       if(BRAM_RST) begin  //reset. set data to 0
                mem[addr] <= {(32){2'b0}};
                // dout <= {(32){2'b0}};
            end
            else
       */ 
    end

endmodule
