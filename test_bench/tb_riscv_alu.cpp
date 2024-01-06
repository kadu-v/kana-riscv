#include <verilated.h>

#include <format>
#include <iostream>

#include "Vriscv_alu.h"
#include "assert.h"
#include "riscv_defs.h"

void test_alu(std::string test_name, uint32_t exec_fun, uint32_t data1,
              uint32_t data2, uint32_t expected) {
  // Instantiate DUT
  Vriscv_alu* dut = new Vriscv_alu();

  dut->exec_fun = exec_fun;
  dut->data1 = data1;
  dut->data2 = data2;
  dut->eval();
  assert_eq(std::format("[alu] {}", test_name), dut->alu_out, expected);
}

void test_branch_flag(std::string test_name, uint32_t data1, uint32_t data2,
                      uint32_t expected) {
  Vriscv_alu* dut = new Vriscv_alu();

  dut->exec_fun = 11; /* ALU_BEQ */
  dut->data1 = data1;
  dut->data2 = data2;
  dut->eval();
  assert_eq(std::format("[branch flag] {}", test_name), dut->br_flag, expected);
}

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);

  // Instantiate DUT
  Vriscv_alu* dut = new Vriscv_alu();

  // initiazlie decoder
  /* do nothing */

  // set input instractions
  // invalid instruction
  test_alu("invalid", 0 /* ALU_X */, 0 /* data1 */, 0 /* data2 */,
           0 /* expected */);

  // check add instruction
  test_alu("ALU_ADD 1 + 2 == 3", 1 /* ALU_ADD */, 1 /* data1 */, 2 /* data2 */,
           3 /* expected */);

  // check sub instruction
  test_alu("ALU_SUB 1 - 2 == -1", 2 /* ALU_SUB */, 1 /* data1 */, 2 /* data2 */,
           -1 /* expected */);

  // check sll instruction
  test_alu("ALU_SLL 0b1 << 10 == 0b10000000000", 3 /* ALU_SLL*/,
           0b1 /* data1 */, 10 /* data2 */, 0b10000000000);

  // check slt instruction
  test_alu("ALU_SLT 100 <s 100 == 0", 4 /* ALU_SLT */, 100 /* data1 */,
           100 /* data2 */, 0 /* expected */);
  test_alu("ALU_SLT 200 <s 100 == 0", 4 /* ALU_SLT */, 200 /* data1 */,
           100 /* data2 */, 0 /* expected */);
  test_alu("ALU_SLT 100 <s -200 == 0", 4 /* ALU_SLT */, 100 /* data1 */,
           -200 /* data2 */, 0 /* expected */);
  test_alu("ALU_SLT 100 <s 200 == 1", 4 /* ALU_SLT */, 100 /* data1 */,
           200 /* data2 */, 1 /* expected */);
  test_alu("ALU_SLT -100 <s 200 == 1", 4 /* ALU_SLT */, -100 /* data1 */,
           200 /* data2 */, 1 /* expected */);

  // check sltu instruction
  test_alu("ALU_SLTU 100 <u 200 == 1", 5 /* ALU_SLTU*/, 100 /* data1 */,
           200 /* data2 */, 1 /* expected */);
  test_alu("ALU_SLTU 100 <u -300 == 1", 5 /* ALU_SLTU*/, 100 /* data1 */,
           -300 /* data2 */, 1 /* expected */);
  test_alu("ALU_SLTU 100 <u 100 == 0", 5 /* ALU_SLTU*/, 100 /* data1 */,
           100 /* data2 */, 0 /* expected */);
  test_alu("ALU_SLTU 300 <u 100 == 0", 5 /* ALU_SLTU*/, 300 /* data1 */,
           100 /* data2 */, 0 /* expected */);
  test_alu("ALU_SLTU -100 <u 200 == 0", 5 /* ALU_SLTU*/, -100 /* data1 */,
           200 /* data2 */, 0 /* expected */);

  // check xor instruction
  test_alu("ALU_XOR 100 <u 200 == 1", 6 /* ALU_XOR */,
           0b111010101010 /* data1 */, 0b010101010101 /* data2 */,
           0b101111111111 /* expected */);

  // check srl instruction
  test_alu("ALU_SRL 0b10000000000 >>u 10 == 0b1", 7 /* ALU_SRL */,
           0b10000000000 /* data1 */, 10 /* data2 */, 0b1 /* expected */);

  // check sra instruction
  test_alu("ALU_SRA 0b10000000000 >>s 10 == 0b1", 8 /* ALU_SRL */,
           0b10000000000 /* data1 */, 10 /* data2 */, 0b1 /* expected */);

  // check or instuction
  test_alu("ALU_OR 0b1010101010 | 0b0101010101 == 0b1111111111", 9 /* ALU_OR */,
           0b1010101010 /* data1 */, 0b0101010101 /* data2 */,
           0b1111111111 /* expected */);

  // check and instruction
  test_alu("ALU_AND 0b1110101010 & 0b0101010101 == 0b0100000000",
           10 /* ALU_AND */, 0b1110101010 /* data1 */, 0b0101010101 /* data2 */,
           0b0100000000 /* expected */);

  // check jalr instruction
  test_alu("ALU_JALR 10 + 21 == 30", 17 /* ALU_JALR */, 10 /* data1 */,
           21 /* data2 */, 30 /* expected */);

  // check branch flag
  test_branch_flag("x[rs1] == x[rs2]", 1 /* data1 */, 1 /* data2 */,
                   1 /* expected */);
  test_branch_flag("x[rs1] == x[rs2]", 1 /* data1 */, 2 /* data2 */,
                   0 /* expected */);
}