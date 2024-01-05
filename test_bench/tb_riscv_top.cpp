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
// x[4] = 100
// x[5] = 200
// x[6] = -100
// x[7] = 0b1010101010
// x[8] = 0b0101010101
// x[9] = 0b1110101010
// x[10] = 1
// x[11] = 10
// x[12] = 0b10000000000
void test_r_type_instruction(std::string test_name, uint32_t inst,
                             uint32_t expected) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, inst);
  tester->set_reg(4, 100);
  tester->set_reg(5, 200);
  tester->set_reg(6, -100);
  tester->set_reg(7, 0b1010101010);
  tester->set_reg(8, 0b0101010101);
  tester->set_reg(9, 0b1110101010);
  tester->set_reg(10, 0b1);
  tester->set_reg(11, 10);
  tester->set_reg(12, 0b10000000000);

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
// 0: jal 1, 8
// 4: addi 2, 0, 10
// 8: addi 3, 0, 20
void test_jal(std::string test_name) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, jal(1, 8));
  tester->set_ram(4, addi(2, 0, 10));
  tester->set_ram(8, addi(3, 0, 20));

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
// 0: beq 1, 2, 8
// 4: addi 2, 0, 10
// 8: addi 3, 0, 20
void test_beq(std::string test_name) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, beq(1, 2, 8));
  tester->set_ram(4, addi(2, 0, 10));
  tester->set_ram(8, addi(3, 0, 20));

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

/* integration　test 1 -------------------------------------------------------*/
// 0: addi 31, 0, 10 // x[31] = x[0] + sext(10) == x[31] = 0 + 10
// 4: addi 20, 0, 20 // x[20] = x[0] + sext(10) == x[20] = 0 + 20
// 8: add 1, 31, 20  // x[1] = x[31] + x[20]    == x[1]  = 10 + 20
// 12: sw 0, 1, 10    // M[ x[0] + sext(10) ] = x[1] == M[10] = 30
// 16: lw 2, 0, 10    // x[2] = M[ x[0] + sext(10) ] == x[2] = M[ 10 ]
void integration_test1(std::string test_name) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, addi(31, 0, 10));
  tester->set_ram(4, addi(20, 0, 20));
  tester->set_ram(8, add(1, 31, 20));
  tester->set_ram(12, sw(0, 1, 10));
  tester->set_ram(16, lw(2, 0, 10));

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

/* integration　test 2 -------------------------------------------------------*/
// 0: addi 31, 0, 10 // x[31] = x[0] + sext(10) == x[31] = 0 + 10
// 4: addi 20, 0, 10 // x[20] = x[0] + sext(10) == x[20] = 0 + 10
// 8: beq 31, 20, 8  // if (x[31] == x[20]) then pc += sext(8)
// 12: addi 1, 0, 100
// 16: addi 2, 0, 200
void integration_test2(std::string test_name) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, addi(31, 0, 10));
  tester->set_ram(4, addi(20, 0, 10));
  tester->set_ram(8, beq(31, 20, 8));
  tester->set_ram(12, addi(1, 0, 100));
  tester->set_ram(16, addi(2, 0, 200));

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
  assert_eq(test_name, tester->get_reg(1), 0);
  assert_eq(test_name, tester->get_reg(2), 200);
}

/* integration　test 3 -------------------------------------------------------*/
//  0: main:   addi x2, x0, 5
//  4:         addi x3, x0, 12
//  8:         addi x7, x3, -9
// 12:         or x4, x7, x2
// 16:         and x5, x3, x4
// 20:         add x5, x5, x4
// 24:         beq x5, x7, end    // imm = 28
// 28:         slt x4, x3, x4
// 32:         beq x4, x0, around // imm = 8
// 36:         addi x5, x0, 0
// 40: around: slt x4, x7, x2　
// 44:         add x7, x4, x5
// 48:         sub x7, x7, x2
// 52:         sw x7, 84(x3)      // sw rs2, offset(rs1)
// 56:         lw x2, 96(x0)      // lw rd, offset(rs1)
// 60:         add x9, x2, x5
// 64:         jal x3, end        // imm = 8
// 68:         addi x2, x0, 1
// 72: end:    add x2, x2, x9
// 76:         sw x2, 0x20(x3)   // imm = 0x20 = 32
// 80: done:   beq x2, x2, done  // imm = 0
void integration_test3(std::string test_name) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, addi(2, 0, 5));
  tester->set_ram(4, addi(3, 0, 12));
  // 0b000000001001 == 9
  // 0b111111110111 == -9
  tester->set_ram(8, addi(7, 3, 0b111111110111));  // imm = -9
  tester->set_ram(12, ior(4, 7, 2));
  tester->set_ram(16, iand(5, 3, 4));
  tester->set_ram(20, add(5, 5, 4));
  tester->set_ram(24, beq(5, 7, 28));
  tester->set_ram(28, slt(4, 3, 4));
  tester->set_ram(32, beq(4, 0, 8));
  tester->set_ram(36, addi(5, 0, 0));
  tester->set_ram(40, slt(4, 7, 2));
  tester->set_ram(44, add(7, 4, 5));
  tester->set_ram(48, sub(7, 7, 2));
  tester->set_ram(52, sw(3, 7, 84));
  tester->set_ram(56, lw(2, 0, 96));
  tester->set_ram(60, add(9, 2, 5));
  tester->set_ram(64, jal(3, 8));
  tester->set_ram(68, addi(2, 0, 1));
  tester->set_ram(72, add(2, 2, 9));
  tester->set_ram(76, sw(3, 2, 32));
  tester->set_ram(80, beq(2, 2, 0));

  // Step 1
  tester->dut_->clk = 0;  // Low
  tester->dut_->x_reset = 1;
  tester->eval();

  for (int i = 0; i < 20; i++) {
    tester->dut_->clk = !tester->dut_->clk;  // High
    tester->eval();

    tester->dut_->clk = !tester->dut_->clk;  // Low
    tester->eval();
  }

  tester->finish();
  assert_eq(test_name, tester->get_ram(100), 25);
}

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);

  /* R type*/
  test_r_type_instruction("[add] x[3] = 100 + 200", add(3, 4, 5), 300);
  test_r_type_instruction("[sub] x[3] = 100 - 200", sub(3, 4, 5), -100);
  test_r_type_instruction("[sll] x[3] = 0b1 << 10", sll(3, 10, 11),
                          0b10000000000);
  test_r_type_instruction("[slt] x[3] = -100 <s 200", slt(3, 6, 5), 1);
  test_r_type_instruction("[sltu] x[3] = 100 <u 200", sltu(3, 4, 5), 1);
  test_r_type_instruction("[xor] x[3] = 0b0101010101 ^ 0b1110101010",
                          ixor(3, 8, 9), 0b1011111111);
  test_r_type_instruction("[srl] x[3] = 0b10000000000 >>u 10", srl(3, 12, 11),
                          0b1);
  test_r_type_instruction("[sra] x[3] = 0b10000000000 >>s 10", srl(3, 12, 11),
                          0b1);
  test_r_type_instruction("[or] x[3] = 0b1010101010 | 0b0101010101",
                          ior(3, 7, 8), 0b1111111111);
  test_r_type_instruction("[and] x[3] = 0b1110101010 | 0b0101010101",
                          iand(3, 9, 8), 0b0100000000);

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
  integration_test1("[integration test1] add, addi, lw, sw");
  integration_test2("[integration test2] addi, addi, beq, addi, addi");
  integration_test3(
      "[integration test] add, sub, slt, or, and, addi, lw, sw, jal, beq");
}
