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
// x[4] = 100
// x[5] = 200
// x[6] = -100
// x[7] = 0b1010101010
// x[8] = 0b0101010101
// x[9] = 0b1110101010
// x[10] = 1
// x[11] = 10
// x[12] = 0b10000000000
// x[13] = 2
// x[14] = 22
// M[8] = expected
// M[12] = 0b10101010_11010101
void test_i_type_instruction(std::string test_name, uint32_t inst,
                             uint32_t expected) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, inst);
  tester->set_ram(8, expected);
  tester->set_ram(12, 0b1010101011010101);
  tester->set_reg(4, 100);
  tester->set_reg(5, 200);
  tester->set_reg(6, -100);
  tester->set_reg(7, 0b1010101010);
  tester->set_reg(8, 0b0101010101);
  tester->set_reg(9, 0b1110101010);
  tester->set_reg(10, 0b1);
  tester->set_reg(11, 10);
  tester->set_reg(12, 0b10000000000);
  tester->set_reg(13, 2);
  tester->set_reg(14, 20);

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

// 0: jalr 3, 1, 8
// 4: addi 2, 0, 10
// 8: addi 3, 0, 20
void test_jalr(std::string test_name) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_reg(1, 4);
  tester->set_ram(0, jalr(4, 1, 4));
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
  assert_ne(test_name, tester->get_reg(2), 10);
  assert_eq(test_name, tester->get_reg(3), 20);
  assert_eq("[jalr] check x[4] = pc + 4", tester->get_reg(4), 4);
}

// CSRs[10] = 10
// x[8] = 9
void test_csr_type_instruction(std::string test_name, uint32_t inst,
                               uint32_t expected, uint32_t csr_expected) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, inst);
  tester->set_csr_reg(10, 10);
  tester->set_reg(8, 9);

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
  assert_eq(test_name, tester->get_csr_reg(10), csr_expected);
}

void test_ecall_instruction() {
  TopTester* tester = new TopTester("[ecall]");
  tester->start();

  // setup
  tester->set_ram(0, ecall());
  tester->set_ram(16, addi(3, 0, 20));
  tester->set_csr_reg(0x305, 16);

  // Step 1
  tester->dut_->clk = 0;
  tester->dut_->x_reset = 1;
  tester->eval();

  for (int i = 0; i < 10; i++) {
    tester->dut_->clk = !tester->dut_->clk;
    tester->eval();
  }
  tester->finish();
  assert_eq("[ecall] x[3] = 20", tester->get_reg(3), 20);
  assert_eq("[ecall]", tester->get_csr_reg(0x342), 11);
}

/* S type--------------------------------------------------------------------*/
// x[0] = expected
// x[1] = 0
// x[2] = 8
// x[3] = 15
// x[4] = 0b11111111_11111111_11010101
void test_s_type_instruction(std::string test_name, uint32_t inst,
                             uint32_t expected) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, inst);
  tester->set_reg(0, expected);
  tester->set_reg(1, 0);
  tester->set_reg(2, 8);
  tester->set_reg(3, 15);
  tester->set_reg(4, 0b111111111111111111010101);

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
  assert_ne(test_name, tester->get_reg(2), 10);
  assert_eq(test_name, tester->get_reg(3), 20);
  assert_eq("[jal] check x[1] = pc + 4", tester->get_reg(1), 4);
}

/* B type--------------------------------------------------------------------*/
// x[1] = 100
// x[2] = 100
// x[3] = 200
// 0: inst
// 4: jal 10, 8
// 8: addi 3, 0, 20
void test_b_type_instruction(std::string test_name, uint32_t inst) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_reg(1, 100);
  tester->set_reg(2, 100);
  tester->set_reg(3, 200);
  tester->set_ram(0, inst);
  tester->set_ram(4, jal(10, 8));
  tester->set_ram(8, addi(5, 0, 20));

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
  assert_eq(test_name, tester->get_reg(5), 20);
}

/* U type--------------------------------------------------------------------*/
// x[1] = 100
// x[2] = 200
// 0: addi 1, 0, 100
// 4: addi 2, 0, 200
// 8: inst
void test_u_type_instruction(std::string test_name, uint32_t inst,
                             uint32_t expected) {
  TopTester* tester = new TopTester(test_name);
  tester->start();

  // setup
  tester->set_ram(0, addi(1, 0, 100));
  tester->set_ram(4, addi(2, 0, 200));
  tester->set_ram(8, inst);

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
  assert_eq(test_name, tester->get_reg(3), expected);
}

/* integration　test 1
   -------------------------------------------------------*/
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

  for (int i = 0; i < 30; i++) {
    tester->dut_->clk = !tester->dut_->clk;  // High
    tester->eval();

    tester->dut_->clk = !tester->dut_->clk;  // Low
    tester->eval();
  }

  tester->finish();
  assert_eq(test_name, tester->get_ram(100), 25);
}

void test_regression(std::string test_name, std::string filename) {
  TopTester* tester =
      new TopTester(std::format("[regression test] {}", test_name));
  std::cout << "[regression test]" << std::endl;

  tester->start();
  tester->set_insts_from_file(filename);

  // Step 1
  tester->dut_->clk = 0;  // Low
  tester->dut_->x_reset = 1;
  tester->eval();

  for (int i = 0; i < 100000; i++) {
    tester->dut_->clk = !tester->dut_->clk;  // High
    tester->eval();

    tester->dut_->clk = !tester->dut_->clk;  // Low
    tester->eval();
  }
  tester->finish();
  assert_eq(test_name, tester->get_reg(3), 1);
  assert_ne(test_name, tester->get_reg(3), 2);
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
  test_r_type_instruction("[sra] x[3] = 0b10000000000 >>s 10", sra(3, 12, 11),
                          0b1);
  test_r_type_instruction("[or] x[3] = 0b1010101010 | 0b0101010101",
                          ior(3, 7, 8), 0b1111111111);
  test_r_type_instruction("[and] x[3] = 0b1110101010 & 0b0101010101",
                          iand(3, 9, 8), 0b0100000000);

  /* I type */
  test_i_type_instruction("[addi] x[3] = 100 + 10 = 110", addi(3, 4, 10), 110);
  test_i_type_instruction("[addi] x[3] = 100 + -2047= -1947",
                          addi(3, 4, 0b100000000001), -1947);
  test_i_type_instruction("[slti] x[3] = 100 <s 200 = 1", slti(3, 4, 200), 1);
  test_i_type_instruction("[sltiu] x[3] = 200 << 500 = 1", sltiu(3, 5, 500), 1);
  test_i_type_instruction("[xori] x[3] = 0b0101010101 ^ 0b1110101010",
                          xori(3, 8, 0b1110101010), 0b1011111111);
  test_i_type_instruction("[ori] x[3] = 0b1010101010 | 0b0101010101",
                          ori(3, 7, 0b0101010101), 0b1111111111);
  test_i_type_instruction("[andi] x[3] = 0b1110101010 | 0b0101010101",
                          andi(3, 9, 0b0101010101), 0b0100000000);
  test_i_type_instruction("[slli] x[3] = 0b1 << 10", slli(3, 10, 10),
                          0b10000000000);
  test_i_type_instruction("[srli] x[3] = 0b10000000000 >>u 10", srli(3, 12, 10),
                          0b1);
  test_i_type_instruction("[srai] x[3] = 0b10000000000 >>s 10", srai(3, 12, 10),
                          0b1);
  // lw
  test_i_type_instruction("[lb] x[3] = sext(M[ x[13] + sext(10) ][7:0])",
                          lb(3, 13, 10), 0b11111111111111111111111111010101);
  test_i_type_instruction("[lh] x[3] = sext(M[ x[13] + sext(10) ][15:0])",
                          lh(3, 13, 10), 0b11111111111111111010101011010101);
  test_i_type_instruction("[lw] x[3] = sext(M[ x[0] + sext(8) ])", lw(3, 0, 8),
                          20);
  test_i_type_instruction("[lw] x[3] = sext(M[ x[13] + sext(6) ])",
                          lw(3, 13, 6), 20);
  test_i_type_instruction("[lw] x[3] = sext(M[ x[14] + sext(-12) ])",
                          lw(3, 14, 0b111111110100 /* -12 */), 20);
  test_i_type_instruction("[lbu] x[3] = M[ x[13] + sext(10) ][7:0]",
                          lbu(3, 13, 10), 0b11010101);
  test_i_type_instruction("[lhu] x[3] = M[ x[13] + sext(10) ][15:0]",
                          lhu(3, 13, 10), 0b1010101011010101);

  test_csr_type_instruction("[csrrw] t=CSRs[10]; CSRs[10]=x[8]; x[3]=t",
                            csrrw(3, 10, 8), 10, 9);
  test_csr_type_instruction("[csrrwi] x[3] = CSRs[10]; CSRs[10] = 100",
                            csrrwi(3, 10, 20), 10, 20);
  // 10 | 9 = 1010 | 1001 = 1011
  test_csr_type_instruction(
      "[csrrs] t = CSRs[10]; CSRs[10] = t | x[8]; x[3] = t", csrrs(3, 10, 8),
      10, 11);
  // 10 | 8 = 1010 | 1000 = 1010
  test_csr_type_instruction(
      "[csrrsi] t = CSRs[10]; CSRs[10] = t | zimm; x[3] = t", csrrsi(3, 10, 8),
      10, 10);
  // 10 & ~9 = 1010 & ~1001 = 1010 & 0110 = 0010
  test_csr_type_instruction(
      "[csrrc] t = CSRs[10]; CSRs[10] = t & ~x[8]; x[3] = t", csrrc(3, 10, 8),
      10, 2);
  // 10 & ~8 = 1010 & ~1000 = 1010 & 0111 = 0010
  test_csr_type_instruction(
      "[csrrci] t = CSRs[10]; CSRs[10] = t & ~zimm; x[3] = t", csrrci(3, 10, 8),
      10, 2);
  test_ecall_instruction();

  /* S type */
  test_s_type_instruction("[sb] M[ x[3] + sext(2) ] = x[4][7:0]",
                          sb(3 /* rs1 (destination) */, 4 /* rs2 (source) */,
                             0b111111111011 /* imm (-5) */),
                          0b11010101);
  test_s_type_instruction("[sh] M[ x[3] + sext(2) ] = x[4][15:0]",
                          sh(3 /* rs1 (destination) */, 4 /* rs2 (source) */,
                             0b111111111011 /* imm (-5) */),
                          0b1111111111010101);
  test_s_type_instruction(
      "[sw] M[ x[1] + sext(10) ] = x[0]",
      sw(1 /* rs1 (destination) */, 0 /* rs2 (source) */, 10 /* imm */), 11);
  test_s_type_instruction(
      "[sw] M[ x[2] + sext(2) ] = x[0]",
      sw(2 /* rs1 (destination) */, 0 /* rs2 (source) */, 2 /* imm */), 12);
  test_s_type_instruction("[sw] M[ x[3] + sext(2) ] = x[0]",
                          sw(3 /* rs1 (destination) */, 0 /* rs2 (source) */,
                             0b111111111011 /* imm (-5) */),
                          13);
  test_jalr("[jalr] jalr 3, 1, 10");

  /* J type */
  // jal
  test_jal("[jal] jal 1, 2; addi 2, 0, 10; addi 3, 0, 20");

  /* B type */
  test_b_type_instruction("[beq] beq 1, 2, 8; jal 10, 8; addi 3, 0, 20",
                          beq(1, 2, 8));
  test_b_type_instruction("[bne] bne 1, 3, 8; jal 10, 8; addi 3, 0, 20",
                          bne(1, 3, 8));
  test_b_type_instruction("[blt] blt 1, 3, 8; jal 10, 8; addi 3, 0, 20",
                          blt(1, 3, 8));
  test_b_type_instruction("[bge] bge 3, 1, 8; jal 10, 8; addi 3, 0, 20",
                          bge(3, 1, 8));
  test_b_type_instruction("[bge] bge 2, 1, 8; jal 10, 8; addi 3, 0, 20",
                          bge(2, 1, 8));
  test_b_type_instruction("[bltu] bltu 1, 3, 8; jal 10, 8; addi 3, 0, 20",
                          bltu(1, 3, 8));
  test_b_type_instruction("[bgeu] bgeu 3, 2, 8; jal 10, 8; addi 3, 0, 20",
                          bgeu(3, 2, 8));

  /* U type */
  test_u_type_instruction("[lui] lui 3, 0b1010", lui(3, 0b1010),
                          0b00000000000000001010000000000000);
  test_u_type_instruction("[auipc] auipc 3, 0b1010", auipc(3, 0b1010),
                          0b00000000000000001010000000000000 + 8);

  /* integration test */
  integration_test1("[integration test1] add, addi, lw, sw");
  integration_test2("[integration test2] addi, addi, beq, addi, addi");
  integration_test3(
      "[integration test] add, sub, slt, or, and, addi, lw, sw, jal, beq");

  test_regression("addi 3, 0, 1", "./boo1.bin");
}
