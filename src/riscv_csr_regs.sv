`include "riscv_constants.sv"

`default_nettype none

module riscv_csr_regs #(
    parameter WORD_LENGTH = 32,
    parameter NUM_REGS = 4096
) (
    /* input */
    input logic                      clk,
    input CSR_WEN                    csr_write_en,
    input logic    [WORD_LENGTH-1:0] csr_addr,
    input logic    [WORD_LENGTH-1:0] csr_data,
    input CSR_FUN                    csr_fun,
    /* output */
    output logic   [WORD_LENGTH-1:0] csr_dout
);
  assign csr_dout = 0;
  logic [WORD_LENGTH-1:0] regs[NUM_REGS];
  // assign csr_dout = csr_fun == CSR_E ? regs[32'h305] : regs[csr_addr];
  // always_ff @(posedge clk) begin
  //   if (csr_write_en && csr_fun == CSR_E) begin
  //     regs[32'h342] <= 11;
  //   end else if (csr_write_en) begin
  //     regs[csr_addr] <= csr_data;
  //   end
  // end
endmodule
