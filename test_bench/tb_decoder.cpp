#include <iostream>
#include <verilated.h>
#include "Vdecoder.h"
#include "riscv_defs.h"
#include "assert.h"

int time_counter = 0;

int main(int argc, char** argv) {

  Verilated::commandArgs(argc, argv);

  // Instantiate DUT
  Vdecoder *dut = new Vdecoder();

  // initiazlie decoder
  /* do nothing */

  // set input instractions
  // add instruction
  dut->inst = INST_ADD; // add instruction
  dut->eval();
  assert_u32("[add instruction]", dut->invalid_o, 1);
}