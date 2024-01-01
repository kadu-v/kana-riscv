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

  dut->imm = 0b00000000000000000000000000000000;
  dut->eval();
  uint32_t expexted = 0;
  assert_eq("sign extend", dut->imm_i_sext, expexted);

  dut->imm = 0b11010101010100000000000000000000;
  dut->eval();
  expexted = 0b11111111111111111111110101010101;
  assert_eq("sign extend", dut->imm_i_sext, expexted);

  dut->imm = 0b01010101010100000000000000000000;
  dut->eval();
  expexted = 0b010101010101;
  assert_eq("sign extend", dut->imm_i_sext, expexted);
}