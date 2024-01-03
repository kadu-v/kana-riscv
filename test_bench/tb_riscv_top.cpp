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
  uint32_t inst = add(3, 2, 1);
  tester->set_ram(0, inst);
  tester->set_reg(1, 100);
  tester->set_reg(2, 200);
  uint32_t expected = 100 + 200;

  // Step 1
  tester->dut_->clk = 0;
  tester->dut_->x_reset = 1;
  tester->eval();

  for (int i = 0; i < 10; i++) {
    tester->dut_->clk = !tester->dut_->clk;
    tester->eval();
  }
  tester->finish();
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
  tester->dut_->x_reset = 1;
  tester->eval();

  for (int i = 0; i < 10; i++) {
    tester->dut_->clk = !tester->dut_->clk;
    tester->eval();
  }
  tester->finish();
  assert_eq(test_name, tester->get_reg(3), expected);
}

void test_lw(std::string test_name, uint32_t inst, uint32_t expected) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, inst);
  tester->set_ram(10, expected);
  tester->set_reg(1, 2);
  tester->set_reg(2, 22);

  // Step 1
  tester->dut_->clk = 0;  // Low
  tester->dut_->x_reset = 1;
  tester->eval();

  tester->dut_->clk = !tester->dut_->clk;  // High
  tester->eval();

  tester->dut_->clk = !tester->dut_->clk;  // Low
  tester->eval();

  tester->finish();
  assert_eq(test_name, tester->get_reg(3), expected);
}

/* S type--------------------------------------------------------------------*/
// x[0] == expected
// x[1] == 15
void test_sw(std::string test_name, uint32_t inst, uint32_t expected) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, inst);
  tester->set_reg(0, expected);
  tester->set_reg(1, 0);
  tester->set_reg(2, 8);
  tester->set_reg(3, 15);

  // Step 1
  tester->dut_->clk = 0;  // Low
  tester->dut_->x_reset = 1;
  tester->eval();

  tester->dut_->clk = !tester->dut_->clk;  // High
  tester->eval();

  tester->dut_->clk = !tester->dut_->clk;  // Low
  tester->eval();

  tester->finish();
  assert_eq(test_name, tester->get_ram(10), expected);
}

/* J type--------------------------------------------------------------------*/
// 0: jal 1, 2
// 1: addi 2, 0, 10
// 2: addi 3, 0, 20
void test_jal(std::string test_name) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, jal(1, 2));
  tester->set_ram(1, addi(2, 0, 10));
  tester->set_ram(2, addi(3, 0, 20));

  // Step 1
  tester->dut_->clk = 0;  // Low
  tester->dut_->x_reset = 1;
  tester->eval();

  for (int i = 0; i < 3; i++) {
    tester->dut_->clk = !tester->dut_->clk;  // High
    tester->eval();

    tester->dut_->clk = !tester->dut_->clk;  // Low
    tester->eval();
  }

  tester->finish();
  assert_eq(test_name, tester->get_reg(3), 20);
  assert_eq("[jal] check x[1] = pc + 4", tester->get_reg(1), 4);
}

/* B type--------------------------------------------------------------------*/
// 0: beq 1, 2, 2
// 1: addi 2, 0, 10
// 2: addi 3, 0, 20
void test_beq(std::string test_name) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, beq(1, 2, 2));
  tester->set_ram(1, addi(2, 0, 10));
  tester->set_ram(2, addi(3, 0, 20));

  // Step 1
  tester->dut_->clk = 0;  // Low
  tester->dut_->x_reset = 1;
  tester->eval();

  for (int i = 0; i < 3; i++) {
    tester->dut_->clk = !tester->dut_->clk;  // High
    tester->eval();

    tester->dut_->clk = !tester->dut_->clk;  // Low
    tester->eval();
  }

  tester->finish();
  assert_eq(test_name, tester->get_reg(3), 20);
}

/* integration
 * test --------------------------------------------------------------------*/
// 0: addi 31, 0, 10 // x[31] = x[0] + sext(10) == x[31] = 0 + 10
// 1: addi 20, 0, 20 // x[20] = x[0] + sext(10) == x[20] = 0 + 20
// 2: add 1, 31, 20  // x[1] = x[31] + x[20]    == x[1]  = 10 + 20
// 3: sw 0, 1, 10    // M[ x[0] + sext(10) ] = x[1] == M[10] = 30
// 4: lw 2, 0, 10    // x[2] = M[ x[0] + sext(10) ] == x[2] = M[ 10 ]
void integration_test1(std::string test_name) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, addi(31, 0, 10));
  tester->set_ram(1, addi(20, 0, 20));
  tester->set_ram(2, add(1, 31, 20));
  tester->set_ram(3, sw(0, 1, 10));
  tester->set_ram(4, lw(2, 0, 10));

  // Step 1
  tester->dut_->clk = 0;  // Low
  tester->dut_->x_reset = 1;
  tester->eval();

  for (int i = 0; i < 10; i++) {
    tester->dut_->clk = !tester->dut_->clk;  // High
    tester->eval();

    tester->dut_->clk = !tester->dut_->clk;  // Low
    tester->eval();
  }

  tester->finish();
  assert_eq(test_name, tester->get_reg(2), 30);
}

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);

  /* R type*/
  // add
  test_add("[add] 100 + 200 = 300");

  /* I type */
  // addi
  test_addi("[addi] 10 + 100 = 110", addi(3, 2, 10), 110);
  test_addi("[addi] -2047 + 100 = -1947", addi(3, 2, 0b100000000001), -1947);
  // lw
  test_lw("[lw] x[3] = sext(M[ x[0] + sext(10) ])", lw(3, 0, 10), 20);
  test_lw("[lw] x[3] = sext(M[ x[1] + sext(8) ])", lw(3, 1, 8), 20);
  test_lw("[lw] x[3] = sext(M[ x[2] + sext(-12) ])",
          lw(3, 2, 0b111111110100 /* -12 */), 20);

  /* S type */
  // sw
  test_sw("[sw] M[ x[1] + sext(10) ] = x[0]",
          sw(1 /* rs1 (destination) */, 0 /* rs2 (source) */, 10 /* imm */),
          11);
  test_sw("[sw] M[ x[2] + sext(2) ] = x[0]",
          sw(2 /* rs1 (destination) */, 0 /* rs2 (source) */, 2 /* imm */), 12);
  test_sw("[sw] M[ x[3] + sext(2) ] = x[0]",
          sw(3 /* rs1 (destination) */, 0 /* rs2 (source) */,
             0b111111111011 /* imm (-5) */),
          13);

  /* S type */
  // jal
  test_jal("[jal] jal 1, 2; addi 2, 0, 10; addi 3, 0, 20");

  /* B type */
  test_beq("[beq] beq 1, 2, 2; addi 2, 0, 10; addi 3, 0, 20");

  /* integration test */
  integration_test1("[integration test] add, addi, lw, sw");
}
