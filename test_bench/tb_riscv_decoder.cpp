#include <verilated.h>

#include <iostream>

#include "Vriscv_decoder.h"
#include "assert.h"
#include "riscv_defs.h"

int time_counter = 0;

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);

  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // initiazlie decoder
  /* do nothing */

  // set input instractions
  // invalid instruction
  dut->inst = 0;
  dut->eval();
  assert_u32("[invalid] check validation of an instruction", dut->invalid_o,
             0 /* invalid */);
  assert_u32("[invalid] check exec_fun", dut->exec_fun, 0 /* ALU_X */);
  assert_u32("[invalid] check op1_sel", dut->op1_sel, 1 /* OP1_RS1 */);
  assert_u32("[invalid] check op2_sel", dut->op2_sel, 1 /* OP2_RS" */);

  // add instruction
  dut->inst = add(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  assert_u32("[add] check validation of an instruction", dut->invalid_o,
             1 /* valid */);
  assert_u32("[add] check exec_fun", dut->exec_fun, 1 /* ALU_ADD */);
  assert_u32("[add] check op1_sel", dut->op1_sel, 1 /* OP1_RS1 */);
  assert_u32("[add] check op2_sel", dut->op2_sel, 1 /* OP2_RS" */);
}