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
  assert_eq("[invalid] check op1_sel", dut->op1_sel, 0 /* OP1_X */);
  assert_eq("[invalid] check op2_sel", dut->op2_sel, 0 /* OP2_X" */);
  assert_eq("[invalid] check wb_sel", dut->wb_sel, 0 /* WB_X */);
  assert_eq("[invalid] check rf_wen", dut->rf_wen, 0 /* RF_X */);
  assert_eq("[invalid] check pc_sel", dut->pc_sel, 0 /* PC_PLUS4 */);

  // add instruction
  dut->inst = add(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  assert_eq("[add] check validation of an instruction", dut->invalid_o,
            1 /* valid */);
  assert_eq("[add] check exec_fun", dut->exec_fun, 1 /* ALU_ADD */);
  assert_eq("[add] check op1_sel", dut->op1_sel, 1 /* OP1_RS1 */);
  assert_eq("[add] check op2_sel", dut->op2_sel, 1 /* OP2_RS" */);
  assert_eq("[add] check wb_sel", dut->wb_sel, 1 /* WB_ALU */);
  assert_eq("[add] check rf_wen", dut->rf_wen, 1 /* RF_WRITE */);
  assert_eq("[add] check pc_sel", dut->pc_sel, 0 /* PC_PLUS4 */);

  // addi instruction
  dut->inst = addi(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  assert_eq("[addi] check validation of an instruction", dut->invalid_o,
            1 /* valid */);
  assert_eq("[addi] check exec_fun", dut->exec_fun, 1 /* ALU_ADD */);
  assert_eq("[addi] check op1_sel", dut->op1_sel, 1 /* OP1_RS1 */);
  assert_eq("[addi] check op2_sel", dut->op2_sel, 2 /* OP2_IMS */);
  assert_eq("[addi] check wb_sel", dut->wb_sel, 1 /* WB_ALU */);
  assert_eq("[addi] check rf_wen", dut->rf_wen, 1 /* RF_WRITE */);
  assert_eq("[addi] check pc_sel", dut->pc_sel, 0 /* PC_PLUS4 */);
}