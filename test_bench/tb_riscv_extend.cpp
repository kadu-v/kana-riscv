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

  /* I type */
  dut->inst = 0b00000000000000000000000000000000;
  dut->eval();
  uint32_t expected = 0;
  assert_eq("[I type] zero check", dut->imm_i_sext, expected);

  dut->inst = 0b11010101010100000000000000000000;
  dut->eval();
  expected = 0b11111111111111111111110101010101;
  assert_eq("[I type] negative value check", dut->imm_i_sext, expected);

  dut->inst = 0b01010101010100000000000000000000;
  dut->eval();
  expected = 0b010101010101;
  assert_eq("[I type] positive value check", dut->imm_i_sext, expected);

  /* S type */
  dut->inst = 0b00000000000000000000000000000000;
  dut->eval();
  expected = 0;
  assert_eq("[S type] zero check", dut->imm_s_sext, expected);

  dut->inst = 0b11010100000000000000111110000000;
  dut->eval();
  expected = 0b11111111111111111111110101011111;
  assert_eq("[S type] negative value check", dut->imm_s_sext, expected);

  dut->inst = 0b01010100000000000000111110000000;
  dut->eval();
  expected = 0b010101011111;
  assert_eq("[S type] positive value check", dut->imm_s_sext, expected);

  /* J type */
  // dut->inst = 0b1_0101010101_1_11010101_000000000000;
  dut->inst = 0b10101010101111010101000000000000;
  dut->eval();
  expected = 0b11111111111111010101101010101010;
  assert_eq("[J type] negative value check", dut->imm_j_sext, expected);

  /* B type*/
  // dut->inst = 0b1_010101_00000_00000_000_1010_1_0000000;
  dut->inst = 0b10101010000000000000101010000000;
  dut->eval();
  expected = 0b11111111111111111111101010110100;
  assert_eq("[B type] negative value check", dut->imm_b_sext, expected);

  /* B type*/
  dut->inst = 0b10101010101010101010000001111111;
  dut->eval();
  expected = 0b10101010101010101010000000000000;
  assert_eq("[U type] negative value check", dut->imm_u_sext, expected);
}