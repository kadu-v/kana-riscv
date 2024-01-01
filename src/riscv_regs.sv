module riscv_regs #(
    parameter WORD_LENGTH = 32,
    parameter ADDR_LENGTH = 5,
    parameter NUM_REGS = 32
) (
    input logic clk,
    input logic write_en,
    input logic [ADDR_LENGTH-1:0] read_addr1,
    input logic [ADDR_LENGTH-1:0] read_addr2,
    input logic [WORD_LENGTH-1:0] data,
    input logic [ADDR_LENGTH-1:0] write_addr,

    output logic [WORD_LENGTH-1:0] read_data1,
    output logic [WORD_LENGTH-1:0] read_data2,
    output logic [WORD_LENGTH-1:0] debug_out
);
  logic [WORD_LENGTH-1:0] regs[NUM_REGS];
  assign read_data1 = regs[read_addr1];
  assign read_data2 = regs[read_addr2];
  assign debug_out  = regs[3];
  always_ff @(posedge clk) begin
    if (write_en) begin
      regs[write_addr] <= data;
    end else begin
      regs[1] <= 1;
      regs[2] <= 2;
    end
  end
endmodule
