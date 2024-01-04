#include <stdint.h>

#include <bitset>
#include <iostream>
#include <sstream>

/* --------------------------------------------------------------------- *
    Utils type
 * ---------------------------------------------------------------------*/
void printBinaryWithPadding32(int number) {
  std::bitset<32> binaryRepresentation(number);
  std::cout << binaryRepresentation << std::endl;
}

/* --------------------------------------------------------------------- *
    R type
 * ---------------------------------------------------------------------*/
#define INST_R_MASK 0b11111110000000000111000001111111

uint32_t r_inst(uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3,
                uint32_t rd, uint32_t opcode) {
  uint32_t inst = funct7 << 25;
  inst += (rs2 << 20);
  inst += (rs1 << 15);
  inst += (funct3 << 12);
  inst += (rd << 7);
  inst += opcode;
  return inst;
}

/* add */
// clang-format off
#define INST_ADD      0b00000000000000000000000000110011
#define FUNCT7_ADD    0b0000000
#define FUNCT3_ADD    0b000
#define OPCODE_ADD    0b0110011
// clang-format on

// add rd, rs1, rs2
// x[rd] = x[rs1] + x[rs2]
uint32_t add(uint32_t rd, uint32_t rs1, uint32_t rs2) {
  return r_inst(FUNCT7_ADD, rs2, rs1, FUNCT3_ADD, rd, OPCODE_ADD);
}

/* sub */
// clang-format off
#define INST_SUB      0b01000000000000000000000000110011
#define FUNCT7_SUB    0b0100000
#define FUNCT3_SUB    0b000
#define OPCODE_SUB    0b0110011
// clang-format on

// sub rd, rs1, rs2
// x[rd] = x[rs1] - x[rs2]
uint32_t sub(uint32_t rd, uint32_t rs1, uint32_t rs2) {
  return r_inst(FUNCT7_SUB, rs2, rs1, FUNCT3_SUB, rd, OPCODE_SUB);
}

/* slt */
// clang-format off
#define INST_SLT      0b00000000000000000010000000111011
#define FUNCT7_SLT    0b0000000
#define FUNCT3_SLT    0b010
#define OPCODE_SLT    0b0111011
// clang-format on

// slt rd, rs1, rs2
// x[rd] = x[rs1] <s x[rs2]
uint32_t slt(uint32_t rd, uint32_t rs1, uint32_t rs2) {
  return r_inst(FUNCT7_SLT, rs2, rs1, FUNCT3_SLT, rd, OPCODE_SLT);
}

/* --------------------------------------------------------------------- *
    I type
 * ---------------------------------------------------------------------*/
#define INST_I_MASK 0b0000000000000000111000001111111

uint32_t i_inst(uint32_t imm, uint32_t rs1, uint32_t funct3, uint32_t rd,
                uint32_t opcode) {
  uint32_t inst = imm << 20;
  inst += (rs1 << 15);
  inst += (funct3 << 12);
  inst += (rd << 7);
  inst += opcode;
  return inst;
}

/* addi */
// clang-format off
#define INST_ADDI      0b0000000000000000000000000010011
#define FUNCT3_ADDI    0b000
#define OPCODE_ADDI    0b0010011
// clang-format on

// addi rd, rs1, imm
// x[rd] = x[rs1] + sext(imm)
uint32_t addi(uint32_t rd, uint32_t rs1, uint32_t imm) {
  return i_inst(imm, rs1, FUNCT3_ADDI, rd, OPCODE_ADDI);
}

/* addi */
// clang-format off
#define INST_LW      0b0000000000000000010000000000011
#define FUNCT3_LW    0b010
#define OPCODE_LW    0b0000011
// clang-format on

// lw rd, rs1, imm
// x[rd] = M[ x[rs1] + sext(imm) ]
uint32_t lw(uint32_t rd, uint32_t rs1, uint32_t imm_i) {
  return i_inst(imm_i, rs1, FUNCT3_LW, rd, OPCODE_LW);
}

/* --------------------------------------------------------------------- *
    S type
 * ---------------------------------------------------------------------*/
#define INST_S_MASK 0b00000000000000000111000001111111

uint32_t s_inst(uint32_t imm_s2, uint32_t rs2, uint32_t rs1, uint32_t funct3,
                uint32_t imm_s1, uint32_t opcode) {
  uint32_t inst = (imm_s2 << 25);
  inst += (rs2 << 20);
  inst += (rs1 << 15);
  inst += (funct3 << 12);
  inst += (imm_s1 << 7);
  inst += opcode;
  return inst;
}

/* sw */
// clang-format off
#define INST_SW      0b0000000000000000010000000000011
#define FUNCT3_SW    0b010
#define OPCODE_SW    0b0100011
// clang-format on

// sw rs1, rs2, imm
// M[ s[rs1] + sext(imm) ] = x[rs2]
uint32_t sw(uint32_t rs1, uint32_t rs2, uint32_t imm_s) {
  uint32_t imm_s2 = (imm_s) >> 5;
  uint32_t imm_s1 = (imm_s & 0b11111);
  return s_inst(imm_s2, rs2, rs1, FUNCT3_SW, imm_s1, OPCODE_SW);
}

/* --------------------------------------------------------------------- *
    J type
 * ---------------------------------------------------------------------*/
#define INST_J_MASK 0b00000000000000000000000001111111

uint32_t j_inst(uint32_t imm, uint32_t rd, uint32_t opcode) {
  uint32_t imm_j0 = ((imm & 0b100000000000000000000) >> 20);  // imm[20]
  uint32_t imm_j1 = ((imm & 0b000000000011111111110) >> 1);   // imm[10:1]
  uint32_t imm_j2 = ((imm & 0b000000000100000000000) >> 11);  // imm[11]
  uint32_t imm_j3 = ((imm & 0b011111111000000000000) >> 12);  // imm[19:12]
  uint32_t inst = (imm_j0 << 31);
  inst += (imm_j1 << 21);
  inst += (imm_j2 << 20);
  inst += (imm_j3 << 12);
  inst += (rd << 7);
  inst += opcode;
  return inst;
}

/* jal */
// clang-format off
#define INST_JAL      0b00000000000000000000000001101111
#define OPCODE_JAL    0b1101111
// clang-format on

// jal rd, imm
// x[rd] = pc + 4; pc += sext(imm)
uint32_t jal(uint32_t rd, uint32_t imm) { return j_inst(imm, rd, OPCODE_JAL); }

/* --------------------------------------------------------------------- *
    B type
 * ---------------------------------------------------------------------*/
#define INST_B_MASK 0b00000000000000000000000001111111

uint32_t b_inst(uint32_t imm, uint32_t rs2, uint32_t rs1, uint32_t funct3,
                uint32_t opcode) {
  uint32_t imm_b0 = ((imm & 0b100000000000) >> 11);  // imm[12]
  uint32_t imm_b1 = ((imm & 0b001111110000) >> 4);   // imm[10:5]
  uint32_t imm_b2 = ((imm & 0b000000001110) >> 1);   // imm[4:1]
  uint32_t imm_b3 = ((imm & 0b010000000000) >> 10);  // imm[11]
  uint32_t inst = (imm_b0 << 31);
  inst += (imm_b1 << 25);
  inst += (rs2 << 20);
  inst += (rs1 << 15);
  inst += (funct3 << 12);
  inst += (imm_b2 << 8);
  inst += (imm_b3 << 7);
  inst += opcode;
  return inst;
}

/* beq */
// clang-format off
#define INST_BEQ      0b00000000000000000000000001100011
#define FUNCT3_BEQ    0b000
#define OPCODE_BEQ    0b1100011
// clang-format on

// beq rs1, rs2, imm
// if(x[rs1] == x[rs2]) then pc += sext(imm)
uint32_t beq(uint32_t rs1, uint32_t rs2, uint32_t imm) {
  return b_inst(imm, rs2, rs1, FUNCT3_BEQ, OPCODE_BEQ);
}