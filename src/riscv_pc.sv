`include "riscv_constants.sv"

module riscv_pc #(
    parameter WORD_LENGTH = 32,
    parameter PC_OFFSET   = 4
) (
    /* input */
    input  logic                    clk,
    input  logic                    x_reset,
    input  PC_SEL                   pc_sel,
    input  logic  [WORD_LENGTH-1:0] imm_j_sext,
    input  logic  [WORD_LENGTH-1:0] imm_b_sext,
    input  logic                    br_flag,
    /* output */
    output logic  [WORD_LENGTH-1:0] pc_plus4,
    output logic  [WORD_LENGTH-1:0] pc_out
);
  logic [31:0] pc;
  assign pc_out   = pc;
  assign pc_plus4 = pc + PC_OFFSET;

  always_ff @(posedge clk) begin
    if (!x_reset) begin
      pc <= 0;
    end else if (pc_sel == PC_J_TARGET) begin
      pc <= pc + imm_j_sext;
    end else if (pc_sel == PC_B_TARGET && br_flag) begin
      pc <= pc + imm_b_sext;
    end else begin
      pc <= pc + PC_OFFSET;
    end
  end
endmodule
