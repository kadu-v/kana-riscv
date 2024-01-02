`include "riscv_defs.sv"
`include "riscv_constants.sv"

module riscv_decoder (
    /* input */
    input  logic    [31:0] inst,
    /* output */
    output logic           invalid_o,
    output EXEC_FUN        exec_fun,
    output OP1_SEL         op1_sel,
    output OP2_SEL         op2_sel,
    output WB_SEL          wb_sel,
    output RF_WEN          rf_wen,
    output PC_SEL          pc_sel
);
  assign invalid_o = invalid_i;
  logic invalid_i = ((inst & `INST_ADD_MASK) == `INST_ADD) ||
                    ((inst & `INST_ADDI_MASK) == `INST_ADDI);

  // if statement for alu
  always_comb begin
    if ((inst & `INST_ADD_MASK) == `INST_ADD) begin
      exec_fun = ALU_ADD;
      op1_sel  = OP1_RS1;
      op2_sel  = OP2_RS2;
      wb_sel   = WB_ALU;
      rf_wen   = RF_WRITE;
      pc_sel   = PC_PLUS4;
    end else if ((inst & `INST_ADDI_MASK) == `INST_ADDI) begin
      exec_fun = ALU_ADD;
      op1_sel  = OP1_RS1;
      op2_sel  = OP2_IMS;
      wb_sel   = WB_ALU;
      rf_wen   = RF_WRITE;
      pc_sel   = PC_PLUS4;
    end else begin
      exec_fun = ALU_X;
      op1_sel  = OP1_X;
      op2_sel  = OP2_X;
      wb_sel   = WB_X;
      rf_wen   = RF_X;
      pc_sel   = PC_PLUS4;
    end
  end

endmodule
