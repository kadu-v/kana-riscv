`include "riscv_constants.sv"

module riscv_pc (
    /* input */
    input  logic         clk,
    input  logic         x_reset,
    input  PC_SEL        pc_sel,
    input  logic  [31:0] imm_i_sext,
    /* output */
    output logic  [31:0] pc_out
);
  logic [31:0] pc;
  assign pc_out = pc;

  always_ff @(posedge clk) begin
    if (!x_reset) begin
      pc <= 0;
    end else if (pc_sel == PC_PLUS4) begin
      pc <= pc + 1;
    end else if (pc_sel == PC_TARGET) begin
      pc <= pc + imm_i_sext;
    end
  end
endmodule
