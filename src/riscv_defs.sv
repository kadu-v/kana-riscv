/*---------------------------------------------------
Instructions
-----------------------------------------------------*/
// and, or
/* R type--------------------------------------------*/
`define INST_R_MASK 32'b1111111_00000_00000_111_00000_1111111
// add
`define INST_ADD 32'b0000000_00000_00000_000_00000_0110011
// sub
`define INST_SUB 32'b0100000_00000_00000_000_00000_0110011
// sll
`define INST_SLL 32'b0000000_00000_00000_001_00000_0110011
// slt
`define INST_SLT 32'b0000000_00000_00000_010_00000_0110011
// sltu
`define INST_SLTU 32'b0000000_00000_00000_011_00000_0110011
// xor
`define INST_XOR 32'b0000000_00000_00000_100_00000_0110011
// srl
`define INST_SRL 32'b0000000_00000_00000_101_00000_0110011
// sra
`define INST_SRA 32'b0100000_00000_00000_101_00000_0110011
// or
`define INST_OR 32'b0000000_00000_00000_110_00000_0110011
// and
`define INST_AND 32'b0000000_00000_00000_111_00000_0110011


/* I type--------------------------------------------*/
`define INST_I_MASK 32'b00000000000_00000_111_00000_1111111
`define INST_SRXI_MASK 32'b11111100000_00000_111_00000_1111111
// addi
`define INST_ADDI 32'b00000000000_00000_000_00000_0010011
// slti
`define INST_SLTI 32'b00000000000_00000_010_00000_0010011
// sltiu
`define INST_SLTIU 32'b00000000000_00000_011_00000_0010011
// xori
`define INST_XORI 32'b00000000000_00000_100_00000_0010011
// ori
`define INST_ORI 32'b00000000000_00000_110_00000_0010011
// andi
`define INST_ANDI 32'b00000000000_00000_111_00000_0010011
// slli
`define INST_SLLI 32'b00000000000_00000_001_00000_0010011
// srli
`define INST_SRLI 32'b00000000000_00000_101_00000_0010011
// srai
`define INST_SRAI 32'b01000000000_00000_101_00000_0010011

// lw
`define INST_LW 32'b00000000000_00000_010_00000_0000011


/* S type--------------------------------------------*/
`define INST_S_MASK 32'b0000000_00000_00000_111_00000_1111111
// sw
`define INST_SW 32'b0000000_00000_00000_010_00000_0100011


/* J type--------------------------------------------*/
`define INST_J_MASK 32'b00000000000000000000_00000_1111111
// jal
`define INST_JAL 32'b00000000000000000000_00000_1101111


/* B type--------------------------------------------*/
`define INST_B_MASK 32'b0000000_00000_00000_111_00000_1111111
// beq
`define INST_BEQ 32'b0000000_00000_00000_000_00000_1100011
// bne
`define INST_BNE 32'b0000000_00000_00000_001_00000_1100011
// blt
`define INST_BLT 32'b0000000_00000_00000_100_00000_1100011
// bge
`define INST_BGE 32'b0000000_00000_00000_101_00000_1100011
// bltu
`define INST_BLTU 32'b0000000_00000_00000_110_00000_1100011
// bgeu
`define INST_BGEU 32'b0000000_00000_00000_111_00000_1100011



/* U type--------------------------------------------*/
`define INST_U_MASK 32'b00000000000000000000_00000_1111111
// lui
`define INST_LUI 32'b00000000000000000000_00000_0110111
// auipc
`define INST_AUIPC 32'b00000000000000000000_00000_0010111


