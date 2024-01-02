#include <verilated.h>

#include <iostream>

#include "Vriscv_wb_mux.h"
#include "assert.h"
#include "riscv_defs.h"

int time_counter = 0;

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);

  // Instantiate DUT
  Vriscv_wb_mux* dut = new Vriscv_wb_mux();

  dut->wb_sel = 1;  // WB_ALU
  dut->alu_out = 100;
  dut->data = 200;
  dut->eval();
  assert_eq("[wb_mux] check WB_ALU", dut->dout, 100);

  dut->wb_sel = 2;  // WB_MEM
  dut->alu_out = 100;
  dut->data = 200;
  dut->eval();
  assert_eq("[wb_mux] check WB_MEM", dut->dout, 200);
}