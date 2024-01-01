`ifndef RISCV_CONSTANTS
`define RISCV_CONSTANTS

typedef enum {
  ALU_X   = 0,
  ALU_ADD = 1
} EXEC_FUN;

typedef enum {
  OP1_X   = 0,
  OP1_RS1 = 1
} OP1_SEL;

typedef enum {
  OP2_X   = 0,
  OP2_RS2 = 1
} OP2_SEL;

typedef enum {
  WB_ALU = 0,
  WB_MEM = 1
} WB_SEL;

`endif
