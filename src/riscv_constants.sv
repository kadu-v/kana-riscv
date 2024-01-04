`ifndef RISCV_CONSTANTS
`define RISCV_CONSTANTS

typedef enum {
  ALU_X   = 0,
  ALU_ADD = 1,
  ALU_SUB = 2,
  ALU_SLT = 3,
  ALU_OR  = 4,
  ALU_AND = 5
} EXEC_FUN;

typedef enum {
  OP1_X   = 0,
  OP1_RS1 = 1
} OP1_SEL;

typedef enum {
  OP2_X   = 0,
  OP2_RS2 = 1,
  OP2_IMI = 2,  // I type
  OP2_IMS = 3,  // S type
  OP2_IMJ = 4   // J type
} OP2_SEL;

typedef enum {
  WB_X   = 0,
  WB_ALU = 1,
  WB_MEM = 2,
  WB_PC  = 3
} WB_SEL;

typedef enum logic {
  MEM_X     = 0,
  MEM_WRITE = 1
} MEM_WEN;

typedef enum logic {
  RF_X     = 0,
  RF_WRITE = 1
} RF_WEN;

typedef enum {
  PC_PLUS4 = 0,
  PC_J_TARGET = 1,
  PC_B_TARGET = 2
} PC_SEL;

`endif
