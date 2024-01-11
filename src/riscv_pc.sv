`include "riscv_constants.sv"

module riscv_pc #(
    parameter WORD_LENGTH = 32,
    parameter PC_OFFSET   = 4
) (
    /* input */
    input  logic                    clk,
    input  logic                    x_reset,
    input  PC_SEL                   pc_sel,
    input  logic  [WORD_LENGTH-1:0] alu_out,
    input  logic  [WORD_LENGTH-1:0] imm_b_sext,
    input  logic                    br_flag,
    input  logic  [WORD_LENGTH-1:0] mtvec_addr,
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
    end else if (pc_sel == PC_ALU) begin
      pc <= alu_out;
    end else if (pc_sel == PC_B_TARGET && br_flag) begin
      pc <= pc + imm_b_sext;
    end else if (pc_sel == PC_ECALL_TARGET) begin
      pc <= mtvec_addr;
    end else begin
      pc <= pc + PC_OFFSET;
    end
  end
endmodule
