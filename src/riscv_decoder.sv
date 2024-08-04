// `ifndef RISCV_DECODER
// `define RISCV_DECODER

// `include "riscv_defs.sv"
// `include "riscv_constants.sv"

// `default_nettype none

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
    output PC_SEL                     pc_sel,
    output MASK_SEL                   rs2_mask_sel,
    output MASK_SEL                   ram_mask_sel,
    output CSR_FUN                    csr_fun,
    output CSR_WEN                    csr_wen
);
  assign invalid_o = invalid_i;
  logic invalid_i;
  assign invalid_i = ((inst & `INST_R_MASK) == `INST_ADD) || 
                  ((inst & `INST_R_MASK) == `INST_SUB) || 
                  ((inst & `INST_R_MASK) == `INST_SLL) || 
                  ((inst &`INST_R_MASK) == `INST_SLT) ||
                  ((inst &`INST_R_MASK) == `INST_SLTU) ||
                  ((inst &`INST_R_MASK) == `INST_XOR) ||
                  ((inst &`INST_R_MASK) == `INST_SRL) ||
                  ((inst &`INST_R_MASK) == `INST_SRA) ||
                  ((inst &`INST_R_MASK) == `INST_OR) ||
                  ((inst &`INST_R_MASK) == `INST_AND) ||
                  ((inst & `INST_I_MASK) == `INST_JALR) ||
                  ((inst & `INST_I_MASK) == `INST_ADDI) ||
                  ((inst & `INST_I_MASK) == `INST_SLTI) ||
                  ((inst & `INST_I_MASK) == `INST_SLTIU) ||
                  ((inst & `INST_I_MASK) == `INST_XORI) ||
                  ((inst & `INST_I_MASK) == `INST_ORI) ||
                  ((inst & `INST_I_MASK) == `INST_ANDI) ||
                  ((inst & `INST_I_MASK) == `INST_SLLI) ||
                  ((inst & `INST_SRXI_MASK) == `INST_SRLI) ||
                  ((inst & `INST_SRXI_MASK) == `INST_SRAI) ||
                  ((inst & `INST_I_MASK) == `INST_LB) ||
                  ((inst & `INST_I_MASK) == `INST_LH) ||
                  ((inst & `INST_I_MASK) == `INST_LW) ||
                  ((inst & `INST_I_MASK) == `INST_LBU) ||
                  ((inst & `INST_I_MASK) == `INST_LHU) ||
                  ((inst & `INST_ECALL_MASK) == `INST_ECALL) ||
                  ((inst & `INST_S_MASK) == `INST_SB) ||
                  ((inst & `INST_S_MASK) == `INST_SH) ||
                  ((inst & `INST_S_MASK) == `INST_SW) ||
                  ((inst & `INST_J_MASK) == `INST_JAL) ||
                  ((inst & `INST_B_MASK) == `INST_BEQ) ||
                  ((inst & `INST_B_MASK) == `INST_BNE) ||
                  ((inst & `INST_B_MASK) == `INST_BLT) ||
                  ((inst & `INST_B_MASK) == `INST_BGE) ||
                  ((inst & `INST_B_MASK) == `INST_BLTU) ||
                  ((inst & `INST_B_MASK) == `INST_BGEU) ||
                  ((inst & `INST_U_MASK) == `INST_LUI) ||
                  ((inst & `INST_U_MASK) == `INST_AUIPC) ||
                  ((inst & `INST_I_MASK) == `INST_CSRRW) ||
                  ((inst & `INST_I_MASK) == `INST_CSRRWI) ||
                  ((inst & `INST_I_MASK) == `INST_CSRRS) ||
                  ((inst & `INST_I_MASK) == `INST_CSRRSI) ||
                  ((inst & `INST_I_MASK) == `INST_CSRRC) ||
                  ((inst & `INST_I_MASK) == `INST_CSRRCI);

  // if statement for alu
  always_comb begin
    if ((inst & `INST_R_MASK) == `INST_ADD) begin  /* R type */
      exec_fun     = ALU_ADD;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_R_MASK) == `INST_SUB) begin
      exec_fun     = ALU_SUB;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_R_MASK) == `INST_SLL) begin
      exec_fun     = ALU_SLL;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_R_MASK) == `INST_SLT) begin
      exec_fun     = ALU_SLT;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_R_MASK) == `INST_SLTU) begin
      exec_fun     = ALU_SLTU;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_R_MASK) == `INST_XOR) begin
      exec_fun     = ALU_XOR;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_R_MASK) == `INST_SRL) begin
      exec_fun     = ALU_SRL;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_R_MASK) == `INST_SRA) begin
      exec_fun     = ALU_SRA;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_R_MASK) == `INST_OR) begin
      exec_fun     = ALU_OR;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_R_MASK) == `INST_AND) begin
      exec_fun     = ALU_AND;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_I_MASK) == `INST_JALR) begin  /* I type */
      exec_fun     = ALU_JALR;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMI;
      wb_sel       = WB_PC;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_ALU;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_I_MASK) == `INST_ADDI) begin
      exec_fun     = ALU_ADD;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMI;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_I_MASK) == `INST_SLTI) begin
      exec_fun     = ALU_SLT;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMI;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_I_MASK) == `INST_SLTIU) begin
      exec_fun     = ALU_SLTU;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMI;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_I_MASK) == `INST_XORI) begin
      exec_fun     = ALU_XOR;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMI;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_I_MASK) == `INST_ORI) begin
      exec_fun     = ALU_OR;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMI;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_I_MASK) == `INST_ANDI) begin
      exec_fun     = ALU_AND;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMI;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_I_MASK) == `INST_SLLI) begin
      exec_fun     = ALU_SLL;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMI;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_SRXI_MASK) == `INST_SRLI) begin
      exec_fun     = ALU_SRL;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMI;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_SRXI_MASK) == `INST_SRAI) begin
      exec_fun     = ALU_SRA;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMI;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_I_MASK) == `INST_LB) begin
      exec_fun     = ALU_ADD;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMI;
      wb_sel       = WB_MEM;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_B_SEXT;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_I_MASK) == `INST_LH) begin
      exec_fun     = ALU_ADD;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMI;
      wb_sel       = WB_MEM;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_H_SEXT;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_I_MASK) == `INST_LW) begin
      exec_fun     = ALU_ADD;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMI;
      wb_sel       = WB_MEM;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_I_MASK) == `INST_LBU) begin
      exec_fun     = ALU_ADD;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMI;
      wb_sel       = WB_MEM;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_B;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_I_MASK) == `INST_LHU) begin
      exec_fun     = ALU_ADD;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMI;
      wb_sel       = WB_MEM;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_H;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_I_MASK) == `INST_CSRRW) begin
      exec_fun     = ALU_X;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_X;
      wb_sel       = WB_CSR;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_W;
      csr_wen      = CSR_WRITE;
    end else if ((inst & `INST_I_MASK) == `INST_CSRRWI) begin
      exec_fun     = ALU_X;
      op1_sel      = OP1_IMZ;
      op2_sel      = OP2_X;
      wb_sel       = WB_CSR;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_W;
      csr_wen      = CSR_WRITE;
    end else if ((inst & `INST_I_MASK) == `INST_CSRRS) begin
      exec_fun     = ALU_X;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_X;
      wb_sel       = WB_CSR;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_S;
      csr_wen      = CSR_WRITE;
    end else if ((inst & `INST_I_MASK) == `INST_CSRRSI) begin
      exec_fun     = ALU_X;
      op1_sel      = OP1_IMZ;
      op2_sel      = OP2_X;
      wb_sel       = WB_CSR;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_S;
      csr_wen      = CSR_WRITE;
    end else if ((inst & `INST_I_MASK) == `INST_CSRRC) begin
      exec_fun     = ALU_X;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_X;
      wb_sel       = WB_CSR;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_C;
      csr_wen      = CSR_WRITE;
    end else if ((inst & `INST_I_MASK) == `INST_CSRRCI) begin
      exec_fun     = ALU_X;
      op1_sel      = OP1_IMZ;
      op2_sel      = OP2_X;
      wb_sel       = WB_CSR;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_C;
      csr_wen      = CSR_WRITE;
    end else if ((inst & `INST_ECALL_MASK) == `INST_ECALL) begin
      exec_fun     = ALU_X;
      op1_sel      = OP1_X;
      op2_sel      = OP2_X;
      wb_sel       = WB_X;
      rf_wen       = RF_X;
      mem_wen      = MEM_X;
      pc_sel       = PC_ECALL_TARGET;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_E;
      csr_wen      = CSR_WRITE;
    end else if ((inst & `INST_S_MASK) == `INST_SB) begin  /* S type */
      exec_fun     = ALU_ADD;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMS;
      wb_sel       = WB_X;
      rf_wen       = RF_X;
      mem_wen      = MEM_WRITE;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_B;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_S_MASK) == `INST_SH) begin
      exec_fun     = ALU_ADD;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMS;
      wb_sel       = WB_X;
      rf_wen       = RF_X;
      mem_wen      = MEM_WRITE;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_H;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_S_MASK) == `INST_SW) begin
      exec_fun     = ALU_ADD;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_IMS;
      wb_sel       = WB_X;
      rf_wen       = RF_X;
      mem_wen      = MEM_WRITE;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_J_MASK) == `INST_JAL) begin  /* J type */
      exec_fun     = ALU_ADD;
      op1_sel      = OP1_PC;
      op2_sel      = OP2_IMJ;
      wb_sel       = WB_PC;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_ALU;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_B_MASK) == `INST_BEQ) begin  /* B type*/
      exec_fun     = ALU_BEQ;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_X;
      rf_wen       = RF_X;
      mem_wen      = MEM_X;
      pc_sel       = PC_B_TARGET;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_B_MASK) == `INST_BNE) begin
      exec_fun     = ALU_BNE;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_X;
      rf_wen       = RF_X;
      mem_wen      = MEM_X;
      pc_sel       = PC_B_TARGET;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_B_MASK) == `INST_BLT) begin
      exec_fun     = ALU_BLT;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_X;
      rf_wen       = RF_X;
      mem_wen      = MEM_X;
      pc_sel       = PC_B_TARGET;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_B_MASK) == `INST_BGE) begin
      exec_fun     = ALU_BGE;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_X;
      rf_wen       = RF_X;
      mem_wen      = MEM_X;
      pc_sel       = PC_B_TARGET;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_B_MASK) == `INST_BLTU) begin
      exec_fun     = ALU_BLTU;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_X;
      rf_wen       = RF_X;
      mem_wen      = MEM_X;
      pc_sel       = PC_B_TARGET;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_B_MASK) == `INST_BGEU) begin
      exec_fun     = ALU_BGEU;
      op1_sel      = OP1_RS1;
      op2_sel      = OP2_RS2;
      wb_sel       = WB_X;
      rf_wen       = RF_X;
      mem_wen      = MEM_X;
      pc_sel       = PC_B_TARGET;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_U_MASK) == `INST_LUI) begin
      exec_fun     = ALU_ADD;
      op1_sel      = OP1_X;
      op2_sel      = OP2_IMU;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else if ((inst & `INST_U_MASK) == `INST_AUIPC) begin
      exec_fun     = ALU_ADD;
      op1_sel      = OP1_PC;
      op2_sel      = OP2_IMU;
      wb_sel       = WB_ALU;
      rf_wen       = RF_WRITE;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end else begin
      exec_fun     = ALU_X;
      op1_sel      = OP1_X;
      op2_sel      = OP2_X;
      wb_sel       = WB_X;
      rf_wen       = RF_X;
      mem_wen      = MEM_X;
      pc_sel       = PC_PLUS4;
      rs2_mask_sel = MASK_X;
      ram_mask_sel = MASK_X;
      csr_fun      = CSR_X;
      csr_wen      = CSR_WEN_X;
    end
  end
endmodule
// `endif