module riscv_extend #(
    parameter WORD_LENGTH = 32
) (
    /* input */
    input  logic [WORD_LENGTH-1:0] inst,
    /* output */
    output logic [WORD_LENGTH-1:0] imm_i_sext,
    output logic [WORD_LENGTH-1:0] imm_s_sext,
    output logic [WORD_LENGTH-1:0] imm_j_sext,
    output logic [WORD_LENGTH-1:0] imm_b_sext,
    output logic [WORD_LENGTH-1:0] imm_u_sext
);

  /* I type */
  logic [11:0] imm_i;
  assign imm_i = inst[31:20];
  assign imm_i_sext = {{20{imm_i[11]}}, imm_i};

  /* S type */
  logic [11:0] imm_s;
  assign imm_s = {inst[31:25], inst[11:7]};
  assign imm_s_sext = {{20{imm_s[11]}}, imm_s};

  /* J type */
  logic [19:0] imm_j;
  assign imm_j = {inst[31], inst[19:12], inst[20], inst[30:21]};
  // RISC-V ISA does not define the first bit of imm,
  // so the first bit have to be 0.
  assign imm_j_sext = {{11{imm_j[19]}}, imm_j, 1'b0};

  /* B type */
  logic [11:0] imm_b;
  assign imm_b = {inst[31], inst[7], inst[30:25], inst[11:8]};
  assign imm_b_sext = {{19{imm_b[11]}}, imm_b, 1'b0};

  /* U type */
  logic [19:0] imm_u;
  assign imm_u = inst[31:12];
  assign imm_u_sext = {imm_u, {12{1'b0}}};
endmodule
