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
  assert_eq("[invalid] check validation of an instruction", dut->invalid_o,
            0 /* invalid */);
  assert_eq("[invalid] check exec_fun", dut->exec_fun, 0 /* ALU_X */);
  assert_eq("[invalid] check op1_sel", dut->op1_sel, 1 /* OP1_RS1 */);
  assert_eq("[invalid] check op2_sel", dut->op2_sel, 1 /* OP2_RS" */);

  // add instruction
  dut->inst = r_inst(OP_ADD, 0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  assert_eq("[add] check validation of an instruction", dut->invalid_o,
            1 /* valid */);
  assert_eq("[add] check exec_fun", dut->exec_fun, 1 /* ALU_ADD */);
  assert_eq("[add] check op1_sel", dut->op1_sel, 1 /* OP1_RS1 */);
  assert_eq("[add] check op2_sel", dut->op2_sel, 1 /* OP2_RS" */);

  // addi instruction
  dut->inst = i_inst(OP_ADDI, 0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  assert_eq("[addi] check validation of an instruction", dut->invalid_o,
            1 /* valid */);
  assert_eq("[addi] check exec_fun", dut->exec_fun, 1 /* ALU_ADD */);
  assert_eq("[addi] check op1_sel", dut->op1_sel, 1 /* OP1_RS1 */);
  assert_eq("[addi] check op2_sel", dut->op2_sel, 2 /* OP2_IMS */);
}