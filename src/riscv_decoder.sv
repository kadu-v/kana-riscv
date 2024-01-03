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
    output MEM_WEN         mem_wen,
    output PC_SEL          pc_sel
);
  assign invalid_o = invalid_i;
  logic invalid_i = ((inst & `INST_R_MASK) == `INST_ADD) || 
                  ((inst & `INST_I_MASK) == `INST_ADDI) ||
                  ((inst & `INST_I_MASK) == `INST_LW) ||
                  ((inst & `INST_S_MASK) == `INST_SW) ;

  // if statement for alu
  always_comb begin

    if ((inst & `INST_R_MASK) == `INST_ADD) begin  /* R type */
      exec_fun = ALU_ADD;
      op1_sel  = OP1_RS1;
      op2_sel  = OP2_RS2;
      wb_sel   = WB_ALU;
      rf_wen   = RF_WRITE;
      mem_wen  = MEM_X;
      pc_sel   = PC_PLUS4;
    end else if ((inst & `INST_I_MASK) == `INST_ADDI) begin  /* I type */
      exec_fun = ALU_ADD;
      op1_sel  = OP1_RS1;
      op2_sel  = OP2_IMI;
      wb_sel   = WB_ALU;
      rf_wen   = RF_WRITE;
      mem_wen  = MEM_X;
      pc_sel   = PC_PLUS4;
    end else if ((inst & `INST_I_MASK) == `INST_LW) begin
      exec_fun = ALU_ADD;
      op1_sel  = OP1_RS1;
      op2_sel  = OP2_IMI;
      wb_sel   = WB_MEM;
      rf_wen   = RF_WRITE;
      mem_wen  = MEM_X;
      pc_sel   = PC_PLUS4;
    end else if ((inst & `INST_S_MASK) == `INST_SW) begin  /* S type */
      exec_fun = ALU_ADD;
      op1_sel  = OP1_RS1;
      op2_sel  = OP2_IMS;
      wb_sel   = WB_X;
      rf_wen   = RF_X;
      mem_wen  = MEM_WRITE;
      pc_sel   = PC_PLUS4;
    end else begin
      exec_fun = ALU_X;
      op1_sel  = OP1_X;
      op2_sel  = OP2_X;
      wb_sel   = WB_X;
      rf_wen   = RF_X;
      mem_wen  = MEM_X;
      pc_sel   = PC_PLUS4;
    end
  end
endmodule
