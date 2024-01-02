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

/* R type--------------------------------------------------------------------*/
void test_add(std::string test_name) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  uint32_t inst = add(1, 2, 3);
  tester->set_ram(0, inst);
  tester->set_reg(1, 100);
  tester->set_reg(2, 200);
  uint32_t expected = 100 + 200;

  // Step 1
  tester->dut_->clk = 0;
  tester->dut_->write_en = 1;
  tester->dut_->eval();

  for (int i = 0; i < 10; i++) {
    tester->dut_->clk = !tester->dut_->clk;
    tester->dut_->write_en = 1;
    tester->dut_->eval();
  }

  assert_eq(test_name, tester->get_reg(3), expected);
}

/* I type--------------------------------------------------------------------*/
void test_addi(std::string test_name, uint32_t inst, uint32_t expected) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, inst);
  tester->set_reg(2, 100);

  // Step 1
  tester->dut_->clk = 0;
  tester->dut_->write_en = 1;
  tester->dut_->eval();

  for (int i = 0; i < 10; i++) {
    tester->dut_->clk = !tester->dut_->clk;
    tester->dut_->write_en = 1;
    tester->dut_->eval();
  }

  assert_eq(test_name, tester->get_reg(3), expected);
}

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);

  /* R type*/
  test_add("[add] 100 + 200 = 300");

  /* I type */
  test_addi("[addi] 10 + 100 = 110", addi(10, 2, 3), 110);
  test_addi("[addi] -2047 + 100 = -1947", addi(0b100000000001, 2, 3), -1947);
}
