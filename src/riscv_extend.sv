module riscv_extend (
    /* input */
    input  logic [31:0] imm,
    output logic [31:0] imm_i_sext
);
  /* I type */
  assign imm_i_sext = {{20{imm[11]}}, imm[31:20]};
endmodule
