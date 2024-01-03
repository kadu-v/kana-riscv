module riscv_extend (
    /* input */
    input  logic [31:0] inst,
    /* output */
    output logic [31:0] imm_i_sext,
    output logic [31:0] imm_s_sext
);

  /* I type */
  logic [11:0] imm_i;
  assign imm_i = inst[31:20];
  assign imm_i_sext = {{20{imm_i[11]}}, imm_i};

  /* S type */
  logic [11:0] imm_s;
  assign imm_s = {inst[31:25], inst[11:7]};
  assign imm_s_sext = {{20{imm_s[11]}}, imm_s};

endmodule
