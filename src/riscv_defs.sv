/*---------------------------------------------------
Instructions
-----------------------------------------------------*/
/* R type--------------------------------------------*/
`define INST_R_MASK 32'b1111111_00000_00000_111_00000_1111111
// add
`define INST_ADD 32'b0000000_00000_00000_000_00000_0110011


/* I type--------------------------------------------*/
`define INST_I_MASK 32'b00000000000_00000_111_00000_1111111
// addi
`define INST_ADDI 32'b00000000000_00000_000_00000_0010011
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


