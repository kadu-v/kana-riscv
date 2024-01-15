#include <gtest/gtest.h>
#include <stdint.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include <iostream>

#include "Vriscv_extend.h"
#include "assert.h"
#include "riscv_defs.h"

int time_counter = 0;

TEST(EXTEND, ZERO) {
  // Instantiate DUT
  Vriscv_extend* dut = new Vriscv_extend();

  // initiazlie decoder
  /* do nothing */

  /* I type */
  dut->inst = 0b00000000000000000000000000000000;
  dut->eval();
  uint32_t expected = 0;
  EXPECT_EQ(dut->imm_i_sext, expected);
}

TEST(EXTEND, ITYPE1) {
  // Instantiate DUT
  Vriscv_extend* dut = new Vriscv_extend();
  dut->inst = 0b11010101010100000000000000000000;
  dut->eval();
  uint32_t expected = 0b11111111111111111111110101010101;
  EXPECT_EQ(dut->imm_i_sext, expected);
}

TEST(EXTEND, ITYP2) {
  // Instantiate DUT
  Vriscv_extend* dut = new Vriscv_extend();
  dut->inst = 0b01010101010100000000000000000000;
  dut->eval();
  uint32_t expected = 0b010101010101;
  EXPECT_EQ(dut->imm_i_sext, expected);
}

TEST(EXTEND, STYPE1) {
  // Instantiate DUT
  Vriscv_extend* dut = new Vriscv_extend();
  /* S type */
  dut->inst = 0b00000000000000000000000000000000;
  dut->eval();
  uint32_t expected = 0;
  EXPECT_EQ(dut->imm_s_sext, expected);
}

TEST(EXTEND, STYPE2) {
  // Instantiate DUT
  Vriscv_extend* dut = new Vriscv_extend();
  dut->inst = 0b11010100000000000000111110000000;
  dut->eval();
  uint32_t expected = 0b11111111111111111111110101011111;
  EXPECT_EQ(dut->imm_s_sext, expected);
}

TEST(EXTEND, STYPE3) {
  // Instantiate DUT
  Vriscv_extend* dut = new Vriscv_extend();
  dut->inst = 0b01010100000000000000111110000000;
  dut->eval();
  uint32_t expected = 0b010101011111;
  EXPECT_EQ(dut->imm_s_sext, expected);
}

TEST(EXTEND, JTYPE) {
  // Instantiate DUT
  Vriscv_extend* dut = new Vriscv_extend();
  /* J type */
  // dut->inst = 0b1_0101010101_1_11010101_000000000000;
  dut->inst = 0b10101010101111010101000000000000;
  dut->eval();
  uint32_t expected = 0b11111111111111010101101010101010;
  EXPECT_EQ(dut->imm_j_sext, expected);
}

TEST(EXTEND, BTYPE1) {
  // Instantiate DUT
  Vriscv_extend* dut = new Vriscv_extend();
  /* B type*/
  // dut->inst = 0b1_010101_00000_00000_000_1010_1_0000000;
  dut->inst = 0b10101010000000000000101010000000;
  dut->eval();
  uint32_t expected = 0b11111111111111111111101010110100;
  EXPECT_EQ(dut->imm_b_sext, expected);
}

TEST(EXTEND, BTYPE2) {
  // Instantiate DUT
  Vriscv_extend* dut = new Vriscv_extend();
  /* B type*/
  dut->inst = 0b10101010101010101010000001111111;
  dut->eval();
  uint32_t expected = 0b10101010101010101010000000000000;
  EXPECT_EQ(dut->imm_u_sext, expected);
}