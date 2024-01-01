module riscv_regs #(
    parameter WORD_LENGTH = 32,
    parameter ADDR_LENGTH = 5,
    parameter NUM_REGS = 32
) (
    /* input */
    input  logic                   clk,
    input  logic                   write_en,
    input  logic [ADDR_LENGTH-1:0] read_addr1,
    input  logic [ADDR_LENGTH-1:0] read_addr2,
    input  logic [WORD_LENGTH-1:0] data,
    input  logic [ADDR_LENGTH-1:0] write_addr,
    /* output */
    output logic [WORD_LENGTH-1:0] read_data1,
    output logic [WORD_LENGTH-1:0] read_data2
);
  logic [WORD_LENGTH-1:0] regs[NUM_REGS];
  assign read_data1 = regs[read_addr1];
  assign read_data2 = regs[read_addr2];
  always_ff @(posedge clk) begin
    if (write_en) begin
      regs[write_addr] <= data;
    end
  end
endmodule
