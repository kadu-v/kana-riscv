#include <verilated.h>

#include <iostream>

#include "Vriscv_extend.h"
#include "assert.h"
#include "riscv_defs.h"

int time_counter = 0;

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);

  // Instantiate DUT
  Vriscv_extend* dut = new Vriscv_extend();

  // initiazlie decoder
  /* do nothing */

  // set input instractions
  // invalid instruction
  dut->imm = 0b00000000000000000000100000000000;
  dut->eval();
  uint32_t expexted = 0b11111111111111111111000000000000;
  assert_eq("sign extend", dut->imm_i_sext, expexted);

  dut->imm = 0b01010101010100000000100000000000;
  dut->eval();
  expexted = 0b11111111111111111111010101010101;
  assert_eq("sign extend", dut->imm_i_sext, expexted);

  dut->imm = 0b01010101010100000000000000000000;
  dut->eval();
  expexted = 0b010101010101;
  assert_eq("sign extend", dut->imm_i_sext, expexted);
}