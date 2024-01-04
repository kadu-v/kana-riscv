#include <verilated.h>

#include <iostream>

#include "Vriscv_alu.h"
#include "assert.h"
#include "riscv_defs.h"

int time_counter = 0;

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);

  // Instantiate DUT
  Vriscv_alu* dut = new Vriscv_alu();

  // initiazlie decoder
  /* do nothing */

  // set input instractions
  // invalid instruction
  dut->exec_fun = 0;
  dut->data1 = 0;
  dut->data2 = 0;
  dut->eval();

  // check add instruction
  dut->exec_fun = 1; /* ALU_ADD */
  dut->data1 = 1;
  dut->data2 = 2;
  dut->eval();
  assert_eq("[alu] ALU_ADD 1 2 == 3", dut->alu_out, 3);

  // check sub instruction
  dut->exec_fun = 2; /* ALU_SUB */
  dut->data1 = 1;
  dut->data2 = 2;
  dut->eval();
  assert_eq("[alu] ALU_SUB 1 2 == -1", dut->alu_out, -1);

  // check slt instruction
  dut->exec_fun = 3; /* ALU_SLT */
  dut->data1 = 100;
  dut->data2 = 100;
  dut->eval();
  assert_eq("[alu] ALU_SLT 100 <s 100 == 0", dut->alu_out, 0);

  dut->exec_fun = 3; /* ALU_SLT */
  dut->data1 = 200;
  dut->data2 = 100;
  dut->eval();
  assert_eq("[alu] ALU_SLT 200 <s 100 == 0", dut->alu_out, 0);

  dut->exec_fun = 3; /* ALU_SLT */
  dut->data1 = 100;
  dut->data2 = -200;
  dut->eval();
  assert_eq("[alu] ALU_SLT 100 <s -200 == 0", dut->alu_out, 0);

  dut->exec_fun = 3; /* ALU_SLT */
  dut->data1 = 100;
  dut->data2 = 200;
  dut->eval();
  assert_eq("[alu] ALU_SLT 100 <s 200 == 1", dut->alu_out, 1);

  dut->exec_fun = 3; /* ALU_SLT */
  dut->data1 = -100;
  dut->data2 = 200;
  dut->eval();
  assert_eq("[alu] ALU_SLT -100 <s 200 == 1", dut->alu_out, 1);

  // check or instruction
  dut->exec_fun = 4; /* ALU_OR */
  dut->data1 = 0b1010101010;
  dut->data2 = 0b0101010101;
  dut->eval();
  assert_eq("[alu] ALU_OR 0b1010101010 | 0b0101010101 == 0b1111111111",
            dut->alu_out, 0b1111111111);

  // check and instruction
  dut->exec_fun = 5; /* ALU_AND */
  dut->data1 = 0b1110101010;
  dut->data2 = 0b0101010101;
  dut->eval();
  assert_eq("[alu] ALU_AND 0b1110101010 | 0b0101010101 == 0b0100000000",
            dut->alu_out, 0b0100000000);

  // check branch flag
  dut->exec_fun = 0; /* ALU_X */
  dut->data1 = 1;
  dut->data2 = 1;
  dut->eval();
  assert_eq("[alu] x[rs1] == x[rs2]", dut->br_flag, 1);

  // check branch flag
  dut->exec_fun = 0; /* ALU_X */
  dut->data1 = 1;
  dut->data2 = 2;
  dut->eval();
  assert_eq("[alu] x[rs1] != x[rs2]", dut->br_flag, 0);
}