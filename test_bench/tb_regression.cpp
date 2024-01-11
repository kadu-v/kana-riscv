#include <gtest/gtest.h>
#include <stdint.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include <format>
#include <iostream>
#include <vector>

#include "Vriscv_top.h"
#include "assert.h"
#include "riscv_defs.h"
#include "top_tester.h"

int main(int argc, char** argv) {
  std::string filename = argv[1];
  TopTester* tester = new TopTester(filename);
  tester->start();
  tester->set_insts_from_file(filename);

  // Step 1
  tester->dut_->clk = 0;  // Low
  tester->dut_->x_reset = 0;
  tester->eval();

  tester->dut_->clk = !tester->dut_->clk;  // High
  tester->eval();
  tester->dut_->x_reset = 1;

  int cnt = 0;
  while (cnt < 10000) {
    cnt++;
    tester->dut_->clk = !tester->dut_->clk;  // High
    tester->eval();

    tester->dut_->clk = !tester->dut_->clk;  // Low
    tester->eval();
  }
  tester->finish();
  assert_eq("test", tester->get_reg(3), 1);
  return 0;
}