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
/* add */
// clang-format off
#define INST_ADD      0b00000000000000000000000000110011
#define INST_ADD_MASK 0b11111110000000000111000001111111
// clang-format on

uint32_t add(uint32_t rs2, uint32_t rs1, uint32_t rd) {
  uint32_t inst = 0b0000000 << (32 - 7);
  inst += (rs2 << 20);
  inst += (rs1 << 15);
  inst + (0b000 << 12);
  inst += (rd << 7);
  inst += (0b0110011);
  return inst;
}