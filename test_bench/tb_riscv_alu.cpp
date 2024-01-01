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
  dut->exec_fun = 1; /* add */
  dut->data1 = 1;
  dut->data2 = 2;
  dut->eval();
  assert_eq("[alu] ALU_ADD 1 2", dut->alu_out, 3);
}