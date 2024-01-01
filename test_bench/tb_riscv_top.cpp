#include <verilated.h>
#include <verilated_vcd_c.h>

#include <format>
#include <iostream>

#include "Vriscv_top.h"
#include "assert.h"
#include "riscv_defs.h"

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);

  // Instantiate DUT
  Vriscv_top* dut = new Vriscv_top();

  // initalize tracer
  // Trace DUMP ON
  Verilated::traceEverOn(true);
  VerilatedVcdC* tfp = new VerilatedVcdC;

  dut->trace(tfp, 100);  // Trace 100 levels of hierarchy
  tfp->open("simx.vcd");
  // end to initialize tracer

  int time_counter = 0;
  tfp->dump(time_counter);
  time_counter++;

  // step 1
  dut->clk = 0;  // Low
  dut->write_en = 0;
  dut->riscv_top__DOT__regs__DOT__regs[1] = 1;
  dut->riscv_top__DOT__regs__DOT__regs[2] = 2;
  dut->eval();
  tfp->dump(time_counter);
  time_counter++;

  // step 2
  dut->clk = !dut->clk;  // High
  dut->eval();
  tfp->dump(time_counter);
  time_counter++;

  // step 3
  dut->clk = 0;  // Low
  dut->write_en = 1;
  dut->riscv_top__DOT__ram__DOT__mem[0] = add(1, 2, 3);
  dut->eval();
  tfp->dump(time_counter);
  time_counter++;

  // step 4
  dut->clk = !dut->clk;  // High
  dut->eval();
  tfp->dump(time_counter);
  time_counter++;

  // step 3
  dut->clk = !dut->clk;  // Low
  dut->eval();
  tfp->dump(time_counter);
  time_counter++;
  dut->final();
  tfp->close();
  assert_eq("add 100 200 3", dut->riscv_top__DOT__regs__DOT__regs[3], 300);
}