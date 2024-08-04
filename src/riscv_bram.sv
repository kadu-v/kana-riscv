// https://www.intel.com/content/www/us/en/docs/programmable/683082/23-1/inferring-ram-functions-from-hdl-code.html
// `ifndef RISCV_BRAM
// `define RISCV_BRAM

// `default_nettype none

module riscv_bram #(
    parameter ADDR_LENGTH = 32,
    parameter NUM_MEM = 10 // BRAM size is 3.4Mbit = 105 * 1024 * 32
 ) (
	/* input */
    input wire clk,
    input wire write_en,
    input wire [ADDR_LENGTH-1:0] waddr,
	input wire [7:0] wdata,
    input wire [ADDR_LENGTH-1:0] raddr,
	/* output */
    output wire [7:0] debug,
    output wire [7:0] dout
);
    logic [7:0] mem[NUM_MEM-1:0];
    assign dout = mem[raddr]; // q does get d in this clock 
                               // cycle if we is high
    assign debug = mem[0]; // q does get d in this clock 
    always @(posedge clk) begin
        if (write_en) begin
            mem[waddr] <= wdata; // Why does the official implementation use non-blocking assignment?
		end
    end
endmodule
// `endif