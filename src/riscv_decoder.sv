`include "riscv_defs.sv"
`include "riscv_constants.sv"

module riscv_decoder #(
    parameter WORD_LENGTH = 32
) (
    /* input */
    input  logic    [WORD_LENGTH-1:0] inst,
    /* output */
    output logic                      invalid_o,
    output EXEC_FUN                   exec_fun,
    output OP1_SEL                    op1_sel,
    output OP2_SEL                    op2_sel,
    output WB_SEL                     wb_sel,
    output RF_WEN                     rf_wen,
    output MEM_WEN                    mem_wen,
    output PC_SEL                     pc_sel
);
  assign invalid_o = invalid_i;
  logic invalid_i = ((inst & `INST_R_MASK) == `INST_ADD) || 
                  ((inst & `INST_R_MASK) == `INST_SUB) || 
                  ((inst & `INST_R_MASK) == `INST_SLL) || 
                  ((inst &`INST_R_MASK) == `INST_SLT) ||
                  ((inst &`INST_R_MASK) == `INST_SLTU) ||
                  ((inst &`INST_R_MASK) == `INST_XOR) ||
                  ((inst &`INST_R_MASK) == `INST_SRL) ||
                  ((inst &`INST_R_MASK) == `INST_SRA) ||
                  ((inst &`INST_R_MASK) == `INST_OR) ||
                  ((inst &`INST_R_MASK) == `INST_AND) ||
                  ((inst & `INST_I_MASK) == `INST_ADDI) ||
                  ((inst & `INST_I_MASK) == `INST_LW) ||
                  ((inst & `INST_S_MASK) == `INST_SW) ||
                  ((inst & `INST_J_MASK) == `INST_JAL) ||
                  ((inst & `INST_B_MASK) == `INST_BEQ);

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
    end else if ((inst & `INST_R_MASK) == `INST_SUB) begin
      exec_fun = ALU_SUB;
      op1_sel  = OP1_RS1;
      op2_sel  = OP2_RS2;
      wb_sel   = WB_ALU;
      rf_wen   = RF_WRITE;
      mem_wen  = MEM_X;
      pc_sel   = PC_PLUS4;
    end else if ((inst & `INST_R_MASK) == `INST_SLL) begin
      exec_fun = ALU_SLL;
      op1_sel  = OP1_RS1;
      op2_sel  = OP2_RS2;
      wb_sel   = WB_ALU;
      rf_wen   = RF_WRITE;
      mem_wen  = MEM_X;
      pc_sel   = PC_PLUS4;
    end else if ((inst & `INST_R_MASK) == `INST_SLT) begin
      exec_fun = ALU_SLT;
      op1_sel  = OP1_RS1;
      op2_sel  = OP2_RS2;
      wb_sel   = WB_ALU;
      rf_wen   = RF_WRITE;
      mem_wen  = MEM_X;
      pc_sel   = PC_PLUS4;
    end else if ((inst & `INST_R_MASK) == `INST_SLTU) begin
      exec_fun = ALU_SLTU;
      op1_sel  = OP1_RS1;
      op2_sel  = OP2_RS2;
      wb_sel   = WB_ALU;
      rf_wen   = RF_WRITE;
      mem_wen  = MEM_X;
      pc_sel   = PC_PLUS4;
    end else if ((inst & `INST_R_MASK) == `INST_XOR) begin
      exec_fun = ALU_XOR;
      op1_sel  = OP1_RS1;
      op2_sel  = OP2_RS2;
      wb_sel   = WB_ALU;
      rf_wen   = RF_WRITE;
      mem_wen  = MEM_X;
      pc_sel   = PC_PLUS4;
    end else if ((inst & `INST_R_MASK) == `INST_SRL) begin
      exec_fun = ALU_SRL;
      op1_sel  = OP1_RS1;
      op2_sel  = OP2_RS2;
      wb_sel   = WB_ALU;
      rf_wen   = RF_WRITE;
      mem_wen  = MEM_X;
      pc_sel   = PC_PLUS4;
    end else if ((inst & `INST_R_MASK) == `INST_SRA) begin
      exec_fun = ALU_SRA;
      op1_sel  = OP1_RS1;
      op2_sel  = OP2_RS2;
      wb_sel   = WB_ALU;
      rf_wen   = RF_WRITE;
      mem_wen  = MEM_X;
      pc_sel   = PC_PLUS4;
    end else if ((inst & `INST_R_MASK) == `INST_OR) begin
      exec_fun = ALU_OR;
      op1_sel  = OP1_RS1;
      op2_sel  = OP2_RS2;
      wb_sel   = WB_ALU;
      rf_wen   = RF_WRITE;
      mem_wen  = MEM_X;
      pc_sel   = PC_PLUS4;
    end else if ((inst & `INST_R_MASK) == `INST_AND) begin
      exec_fun = ALU_AND;
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
    end else if ((inst & `INST_J_MASK) == `INST_JAL) begin  /* J type */
      exec_fun = ALU_ADD;
      op1_sel  = OP1_PC;
      op2_sel  = OP2_IMJ;
      wb_sel   = WB_PC;
      rf_wen   = RF_WRITE;
      mem_wen  = MEM_X;
      pc_sel   = PC_ALU;
    end else if ((inst & `INST_B_MASK) == `INST_BEQ) begin  /* B type*/
      exec_fun = ALU_X;
      op1_sel  = OP1_RS1;
      op2_sel  = OP2_RS2;
      wb_sel   = WB_X;
      rf_wen   = RF_X;
      mem_wen  = MEM_X;
      pc_sel   = PC_B_TARGET;
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
