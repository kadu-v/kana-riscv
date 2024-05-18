#include <gtest/gtest.h>
#include <stdint.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include <format>
#include <iostream>
#include <vector>

#include "Vriscv_top.h"
#include "top_tester.h"

std::string BASE_PATH = "./target/bin/";

int regression_test(std::string filename)
{
  TopTester *tester = new TopTester(filename);
  tester->start();
  tester->set_insts_from_file(BASE_PATH + filename);

  // Step 1
  tester->dut_->clk = 0; // Low
  tester->dut_->x_reset = 0;
  tester->eval();

  tester->dut_->clk = !tester->dut_->clk; // High
  tester->eval();
  tester->dut_->x_reset = 1;

  int cnt = 0;
  while (!Verilated::gotFinish() && cnt < 5000)
  {
    cnt++;
    tester->dut_->clk = !tester->dut_->clk; // High
    tester->eval();

    tester->dut_->clk = !tester->dut_->clk; // Low
    tester->eval();
  }
  tester->finish();
  EXPECT_EQ(tester->get_reg(3), 1);
  return 0;
}

TEST(RISCV_RV32_UI, ADD)
{
  std::string filename = "rv32ui-p-add.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, ADDI)
{
  std::string filename = "rv32ui-p-addi.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, AND)
{
  std::string filename = "rv32ui-p-and.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, ANDI)
{
  std::string filename = "rv32ui-p-andi.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, AUIPC)
{
  std::string filename = "rv32ui-p-auipc.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, BEQ)
{
  std::string filename = "rv32ui-p-beq.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, BGE)
{
  std::string filename = "rv32ui-p-bge.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, BGEU)
{
  std::string filename = "rv32ui-p-bgeu.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, BLT)
{
  std::string filename = "rv32ui-p-blt.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, BLTU)
{
  std::string filename = "rv32ui-p-bltu.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, BNE)
{
  std::string filename = "rv32ui-p-bne.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, JAL)
{
  std::string filename = "rv32ui-p-jal.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, JARL)
{
  std::string filename = "rv32ui-p-jalr.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, LB)
{
  std::string filename = "rv32ui-p-lb.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, LBU)
{
  std::string filename = "rv32ui-p-lbu.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, LH)
{
  std::string filename = "rv32ui-p-lh.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, LHU)
{
  std::string filename = "rv32ui-p-lhu.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, LUI)
{
  std::string filename = "rv32ui-p-lui.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, LW)
{
  std::string filename = "rv32ui-p-lw.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, MA_DATA)
{
  std::string filename = "rv32ui-p-ma_data.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, OR)
{
  std::string filename = "rv32ui-p-or.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, ORI)
{
  std::string filename = "rv32ui-p-ori.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, SB)
{
  std::string filename = "rv32ui-p-sb.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, SH)
{
  std::string filename = "rv32ui-p-sh.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, SIMPLE)
{
  std::string filename = "rv32ui-p-simple.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, SLL)
{
  std::string filename = "rv32ui-p-sll.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, SLLI)
{
  std::string filename = "rv32ui-p-slli.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, SLT)
{
  std::string filename = "rv32ui-p-slt.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, SLTI)
{
  std::string filename = "rv32ui-p-slti.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, SLTIU)
{
  std::string filename = "rv32ui-p-sltiu.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, SLTU)
{
  std::string filename = "rv32ui-p-sltu.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, SRA)
{
  std::string filename = "rv32ui-p-sra.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, SRAI)
{
  std::string filename = "rv32ui-p-srai.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, SRL)
{
  std::string filename = "rv32ui-p-srl.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, SRLI)
{
  std::string filename = "rv32ui-p-srli.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, SUB)
{
  std::string filename = "rv32ui-p-sub.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, SW)
{
  std::string filename = "rv32ui-p-sw.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, XOR)
{
  std::string filename = "rv32ui-p-xor.bin";
  regression_test(filename);
}

TEST(RISCV_RV32_UI, XORI)
{
  std::string filename = "rv32ui-p-xori.bin";
  regression_test(filename);
}