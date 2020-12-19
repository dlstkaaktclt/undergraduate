`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/04/24 20:00:57
// Design Name: 
// Module Name: my_pe_controller
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


module my_pe_controller #(
       parameter G_BUF_SIZE = 6, // global buffer size
	   parameter L_RAM_SIZE = 6,  // local buffer size
	   parameter VECTOR_NUM = 4, // vector number. 2**vector_num.
	   parameter integer BRAM_ADDR_WIDTH = 15
    )
    (
         input start,
         input aclk,
         input aresetn,
         input [31:0] rddata,
         output [BRAM_ADDR_WIDTH-1:0] rdaddr,
         output reg [31:0] wrdata,
         output done
    );

// for PE
wire [31:0] ain; // port A

wire [31:0] din; //peram --> port B
wire [L_RAM_SIZE-1:0] addr;
wire we_local;
// integrated valid signal
wire valid;
// computation result
wire dvalid;
wire [31:0] dout;



// for global buffer
wire we_global;
reg [31:0] global_reg;
(* ram_style = "block" *) reg [31:0] global_buffer [0:2**G_BUF_SIZE - 1];


// for FSM
reg [3:0] state;
parameter S_IDLE = 4'b0001, S_LOAD = 4'b0010, S_CALC = 4'b0100, S_DONE = 4'b1000;

wire load_done, calc_done, done_done;
wire load_trigger, calc_trigger, done_trigger;
reg calc_trigger_delay1;

reg calc_input_need;
reg dvalid_delay_1;
//trigger wire is on when present state and next state is differ. it is use to initiate. 
//load_trigger : present state = IDLE, next state = LOAD
//calc_trigger : present state = LOAD, next state = CALC
//done_trigger : present state = CALC, next state = DONE



// for counter

//use down counter for convinience.
reg [31:0] counter;
wire [31:0] counter_load_value;
wire counter_load_signal;
wire counter_reset;
wire counter_enable;




// global buffer part
always @(posedge aclk) begin
        if (we_global) // write enable of global is 1, then write data into global buffer
            global_buffer[addr] <= rddata;
        else
            global_reg <= global_buffer[addr];
end


// counter part

localparam LOAD_CVAL = (2**VECTOR_NUM * 4) - 1;
localparam CALC_CVAL = 2**VECTOR_NUM - 1;
localparam DONE_CVAL = 4; // 5 cycle wait.

assign counter_reset = (!aresetn) | (done_done);
assign counter_load_signal = (load_trigger) | (calc_trigger) | (done_trigger);
assign counter_load_value = (load_trigger) ? LOAD_CVAL :
                            (calc_trigger) ? CALC_CVAL :
                            (done_trigger) ? DONE_CVAL : 32'd0;
assign counter_enable = (state == S_LOAD) | (state == S_CALC && dvalid == 2'b1) | (state == S_DONE);


always @(posedge aclk) begin
    if(counter_reset) begin
        counter <= 32'd1; // I initialize counter to 1 because detecting current state at end whens counter is zero.
        //initializing counter to 1 prevent false-detecting current state at end. 
    end
    else if(counter_load_signal) begin
        counter <= counter_load_value;
    end
    else if(counter_enable) begin
        counter <= counter - 32'd1;
    end
end


    
// FSM part

assign load_trigger = state[0] && start;        // check state is changing..
assign calc_trigger = state[1] && load_done;
assign done_trigger = state[2] && calc_done;



// 1. initialize 

// 2. determine next state
always @(posedge aclk) begin
    if (!aresetn) begin
            state <= S_IDLE;
    end
    else begin
        case (state)
            S_IDLE:
                state <= (start) ? S_LOAD : S_IDLE;
            S_LOAD:
                state <= (load_done) ? S_CALC : S_LOAD;
            S_CALC: 
                state <= (calc_done) ? S_DONE : S_CALC;
            S_DONE:
                state <= (done_done) ? S_IDLE : S_DONE;
            default:
                state <= S_IDLE;
        endcase
    end
end    

// 3. evalutate.


assign ain = global_reg;
assign din = rddata;



// LOAD part.
assign rdaddr = (state == S_LOAD) ? counter[BRAM_ADDR_WIDTH:1] : 'd0; 
assign we_local = ((state == S_LOAD) && (counter[VECTOR_NUM+1])) ? 1'b1 : 1'b0; 
assign we_global = ((state == S_LOAD) && (!counter[VECTOR_NUM+1])) ? 1'b1 : 1'b0;
assign addr = (state == S_LOAD) ? {{(L_RAM_SIZE - VECTOR_NUM){2'b0}} , counter[VECTOR_NUM:1]} : 
              (state == S_CALC) ? counter : 'd0;
assign load_done = (state == S_LOAD && counter == 32'd0);

// CALC part.
always @(posedge aclk) begin
    dvalid_delay_1 <= dvalid;
end

always @(posedge aclk) begin
    calc_trigger_delay1 <= calc_trigger;
end


always @(posedge aclk) begin
    if(!aresetn) begin
        calc_input_need = 1'b0;
    end
    else if(calc_trigger_delay1) begin 
        calc_input_need = 1'b1;
    end
    else if(dvalid_delay_1 && state == S_CALC) begin
        calc_input_need = 1'b1;
    end
    else begin
        calc_input_need = 1'b0;
    end
end
        
assign valid = (state == S_CALC) && calc_input_need;
assign calc_done = (state == S_CALC && counter == 32'd0 && dvalid == 1'b1);

always @(posedge aclk) begin
        if (!aresetn) begin
                wrdata <= 'd0;
        end
        else begin
            if (calc_done) begin
                    wrdata <= dout;
            end
            else begin
                    wrdata <= wrdata;
            end
        end
end

// DONE part.
assign done_done = (state == S_DONE && counter == 32'd0);
assign done = (state == S_DONE);

my_pe #(6) PE (
      .aclk(aclk),
      .aresetn(aresetn),
      .ain(ain),
      .din(din),
      .addr(addr),
      .we(we_local),
      .valid(valid),
      .dvalid(dvalid),
      .dout(dout)
    );




endmodule
