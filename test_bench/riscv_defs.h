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

uint32_t add(uint32_t rs2, uint32_t rs1, uint32_t rd) {
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

uint32_t addi(uint32_t imm, uint32_t rs1, uint32_t rd) {
  return i_inst(imm, rs1, FUNCT3_ADDI, rd, OPCODE_ADDI);
}

/* addi */
// clang-format off
#define INST_LW      0b0000000000000000010000000000011
#define FUNCT3_LW    0b010
#define OPCODE_LW    0b0000011
// clang-format on

uint32_t lw(uint32_t imm, uint32_t rs1, uint32_t rd) {
  return i_inst(imm, rs1, FUNCT3_LW, rd, OPCODE_LW);
}
