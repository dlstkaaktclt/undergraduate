`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/04/10 18:28:18
// Design Name: 
// Module Name: my_pe
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


module my_pe #(parameter L_RAM_SIZE = 6)
(
    // clk /reset
    input aclk,
    input aresetn,
    // port A
    input [31:0] ain,
    //peram --> port B
    input [31:0] din,
    input [L_RAM_SIZE-1:0] addr,
    input we,
    // integrated valid signal
    input valid,
    // computation result
    output dvalid,
    output [31:0] dout
);

(* ram_style = "block" *) reg [31:0] peram [0:2**L_RAM_SIZE - 1]; // local register

reg [31:0] r_dout;
reg [31:0] bin;
wire [31:0] temp_dout;


initial begin
    r_dout <= 0;
    bin <= 0;
end


always @ (posedge aclk) begin
    if (we) begin
        peram[addr] <= din;
    end
    else begin
        bin <= peram[addr];
    end
end

always @ (posedge aclk) begin
    if (!aresetn) begin
        r_dout <= {(32){2'b0}};
    end
    else if(dvalid) begin
        r_dout <= temp_dout;
    end
    else begin
        r_dout <= r_dout;
    end
end

assign dout = temp_dout & {(32){dvalid}};

    fused_mult UUT(
            .aclk(aclk),
            .aresetn(aresetn),
            .s_axis_a_tvalid(valid),
            .s_axis_b_tvalid(valid),
            .s_axis_c_tvalid(valid),
            .s_axis_a_tdata(ain),
            .s_axis_b_tdata(bin),
            .s_axis_c_tdata(r_dout),
            .m_axis_result_tvalid(dvalid),
            .m_axis_result_tdata(temp_dout)
        );

endmodule
