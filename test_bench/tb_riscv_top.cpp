#include <stdint.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include <format>
#include <iostream>
#include <vector>

#include "Vriscv_top.h"
#include "assert.h"
#include "riscv_defs.h"

// void set_instructions(Vriscv_top* dut, std::vector<uint32_t> insts) {
//   int addr = 0;
//   for (int i = 0; i < insts.size(); i++) {
//     dut->riscv_top__DOT__ram__DOT__mem[i] = insts[i];
//   }
// }

void set_register(Vriscv_top* dut, uint32_t addr, uint32_t val) {
  dut->riscv_top__DOT__regs__DOT__regs[addr] = val;
}

void test_instruction(std::string test_name, Vriscv_top* dut, uint32_t inst,
                      uint32_t expected) {
  int time_counter = 0;

  // initalize tracer
  // Trace DUMP ON
  Verilated::traceEverOn(true);
  VerilatedVcdC* tfp = new VerilatedVcdC;
  dut->trace(tfp, 100);  // Trace 100 levels of hierarchy
  tfp->open(std::format("{}.vcd", test_name).c_str());

  // step 1
  dut->clk = 0;  // Low
  dut->write_en = 0;
  dut->eval();
  tfp->dump(time_counter);
  time_counter++;

  // step 2
  dut->clk = !dut->clk;  // High
  dut->eval();
  tfp->dump(time_counter);
  time_counter++;

  // step 3
  dut->clk = !dut->clk;  // Low
  dut->write_en = 1;
  dut->riscv_top__DOT__ram__DOT__mem[0] = inst;
  dut->eval();
  tfp->dump(time_counter);
  time_counter++;

  // step 4
  dut->clk = !dut->clk;  // High
  dut->eval();
  tfp->dump(time_counter);
  time_counter++;

  // step 5
  dut->clk = !dut->clk;  // Low
  dut->eval();
  tfp->dump(time_counter);
  time_counter++;
  dut->final();
  tfp->close();
  assert_eq(test_name, dut->riscv_top__DOT__regs__DOT__regs[3], expected);
}

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);

  // Instantiate DUT
  Vriscv_top* dut = new Vriscv_top();
  uint32_t inst = r_inst(OP_ADD, 1, 2, 3);
  set_register(dut, 1, 100);
  set_register(dut, 2, 200);
  test_instruction("[add] 100 + 200 = 300", dut, inst, 300);

  inst = i_inst(OP_ADDI, 10, 2, 3);
  set_register(dut, 2, 100);
  test_instruction("[addi] 10 + 100 = 110", dut, inst, 110);

  inst = i_inst(OP_ADDI, 0b100000000001, 2, 3);
  set_register(dut, 2, 100);
  test_instruction("[addi] -2047 + 100 = -1947", dut, inst, -1947);
}
