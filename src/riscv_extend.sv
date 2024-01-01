module riscv_extend (
    /* input */
    input  logic [31:0] imm,
    output logic [31:0] imm_i_sext
);
  /* I type */
  logic [11:0] imm_i = imm[31:20];
  assign imm_i_sext = {{20{imm_i[11]}}, imm_i};
endmodule
