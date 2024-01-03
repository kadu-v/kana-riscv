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
uint32_t r_inst(uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3,
                uint32_t rd, uint32_t opcode) {
  uint32_t inst = funct7 << 25;
  inst += (rs2 << 20);
  inst += (rs1 << 15);
  inst + (funct3 << 12);
  inst += (rd << 7);
  inst += opcode;
  return inst;
}

/* add */
// clang-format off
#define INST_ADD      0b00000000000000000000000000110011
#define INST_ADD_MASK 0b11111110000000000111000001111111
#define FUNCT7_ADD    0b0000000
#define FUNCT3_ADD    0b000
#define OPCODE_ADD    0b0110011
// clang-format on

uint32_t add(uint32_t rd, uint32_t rs1, uint32_t rs2) {
  return r_inst(FUNCT7_ADD, rs2, rs1, FUNCT3_ADD, rd, OPCODE_ADD);
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

uint32_t addi(uint32_t rd, uint32_t rs1, uint32_t imm) {
  return i_inst(imm, rs1, FUNCT3_ADDI, rd, OPCODE_ADDI);
}

/* addi */
// clang-format off
#define INST_LW      0b0000000000000000010000000000011
#define FUNCT3_LW    0b010
#define OPCODE_LW    0b0000011
// clang-format on

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

/* sw */
// clang-format off
#define INST_JAL      0b00000000000000000000000001101111
#define OPCODE_JAL    0b1101111
// clang-format on

uint32_t jal(uint32_t rd, uint32_t imm) { return j_inst(imm, rd, OPCODE_JAL); }