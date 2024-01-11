#include <gtest/gtest.h>
#include <stdint.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include <format>
#include <iostream>
#include <vector>

#include "Vriscv_alu.h"

void test_alu(uint32_t exec_fun, uint32_t data1, uint32_t data2,
              uint32_t expected) {
  // Instantiate DUT
  Vriscv_alu* dut = new Vriscv_alu();

  dut->exec_fun = exec_fun;
  dut->data1 = data1;
  dut->data2 = data2;
  dut->eval();
  EXPECT_EQ(dut->alu_out, expected);
}

void test_branch_flag(uint32_t data1, uint32_t data2, uint32_t expected) {
  Vriscv_alu* dut = new Vriscv_alu();

  dut->exec_fun = 11; /* ALU_BEQ */
  dut->data1 = data1;
  dut->data2 = data2;
  dut->eval();
  EXPECT_EQ(dut->br_flag, expected);
}

TEST(ALU, invalid) {
  test_alu(0 /* ALU_X */, 0 /* data1 */, 0 /* data2 */, 0 /* expected */);
}

TEST(ALU, add) {
  test_alu(1 /* ALU_ADD */, 1 /* data1 */, 2 /* data2 */, 3 /* expected */);
}

TEST(ALU, sub) {
  test_alu(2 /* ALU_SUB */, 1 /* data1 */, 2 /* data2 */, -1 /* expected */);
}

TEST(ALU, sll) {
  test_alu(3 /* ALU_SLL*/, 0b1 /* data1 */, 10 /* data2 */, 0b10000000000);
}

TEST(ALU, slt) {
  test_alu(4 /* ALU_SLT */, 100 /* data1 */, 100 /* data2 */, 0 /* expected */);
  test_alu(4 /* ALU_SLT */, 200 /* data1 */, 100 /* data2 */, 0 /* expected */);
  test_alu(4 /* ALU_SLT */, 100 /* data1 */, -200 /* data2 */,
           0 /* expected */);
  test_alu(4 /* ALU_SLT */, 100 /* data1 */, 200 /* data2 */, 1 /* expected */);
  test_alu(4 /* ALU_SLT */, -100 /* data1 */, 200 /* data2 */,
           1 /* expected */);
}

TEST(ALU, sltu) {
  test_alu(5 /* ALU_SLTU*/, 100 /* data1 */, 200 /* data2 */, 1 /* expected */);
  test_alu(5 /* ALU_SLTU*/, 100 /* data1 */, -300 /* data2 */,
           1 /* expected */);
  test_alu(5 /* ALU_SLTU*/, 100 /* data1 */, 100 /* data2 */, 0 /* expected */);
  test_alu(5 /* ALU_SLTU*/, 300 /* data1 */, 100 /* data2 */, 0 /* expected */);
  test_alu(5 /* ALU_SLTU*/, -100 /* data1 */, 200 /* data2 */,
           0 /* expected */);
}

TEST(ALU, XOR) {
  test_alu(6 /* ALU_XOR */, 0b111010101010 /* data1 */,
           0b010101010101 /* data2 */, 0b101111111111 /* expected */);
}

TEST(ALU, srl) {
  test_alu(7 /* ALU_SRL */, 0b10000000000 /* data1 */, 10 /* data2 */,
           0b1 /* expected */);
}

TEST(ALU, sra) {
  test_alu(8 /* ALU_SRL */, 0b10000000000 /* data1 */, 10 /* data2 */,
           0b1 /* expected */);
}

TEST(ALU, OR) {
  test_alu(9 /* ALU_OR */, 0b1010101010 /* data1 */, 0b0101010101 /* data2 */,
           0b1111111111 /* expected */);
}

TEST(ALU, AND) {
  test_alu(10 /* ALU_AND */, 0b1110101010 /* data1 */, 0b0101010101 /* data2 */,
           0b0100000000 /* expected */);
}

TEST(ALU, jalr) {
  test_alu(17 /* ALU_JALR */, 10 /* data1 */, 21 /* data2 */,
           30 /* expected */);
}

TEST(ALU, branch_flag) {
  test_branch_flag(1 /* data1 */, 1 /* data2 */, 1 /* expected */);
  test_branch_flag(1 /* data1 */, 2 /* data2 */, 0 /* expected */);
}