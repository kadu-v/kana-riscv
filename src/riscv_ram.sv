module riscv_ram #(
    parameter WORD_LENGTH = 32,
    parameter ADDR_LENGTH = 5,
    parameter NUM_MEM = 4096
) (
    input  logic                   clk,
    /* port for instruction */
    input  logic [WORD_LENGTH-1:0] pc,
    output logic [WORD_LENGTH-1:0] inst,
    /* port for data */
    input  logic [WORD_LENGTH-1:0] addr,
    input  logic                   write_en,
    input  logic [WORD_LENGTH-1:0] wdata,
    output logic [WORD_LENGTH-1:0] dout
);
  logic [WORD_LENGTH-1:0] mem[NUM_MEM];

  assign inst = mem[pc];
  assign dout = mem[addr];

  always_ff @(posedge clk) begin
    if (write_en) begin
      mem[addr] <= wdata;
    end
  end


endmodule
