#include <gtest/gtest.h>
#include <stdint.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include <iostream>

#include "Vriscv_wb_mux.h"
#include "assert.h"
#include "riscv_defs.h"

int time_counter = 0;

TEST(WBMUX, ALU) {
  Vriscv_wb_mux* dut = new Vriscv_wb_mux();

  dut->wb_sel = 1;  // WB_ALU
  dut->alu_out = 100;
  dut->data = 200;
  dut->pc_plus4 = 300;
  dut->eval();
  EXPECT_EQ(dut->dout, 100);
}

TEST(WBMUX, MEM) {
  // Instantiate DUT
  Vriscv_wb_mux* dut = new Vriscv_wb_mux();

  dut->wb_sel = 2;  // WB_MEM
  dut->alu_out = 100;
  dut->data = 200;
  dut->pc_plus4 = 300;
  dut->eval();
  EXPECT_EQ(dut->dout, 200);
}

TEST(WBMUX, PC) {
  Vriscv_wb_mux* dut = new Vriscv_wb_mux();

  dut->wb_sel = 3;  // WB_PC
  dut->alu_out = 100;
  dut->data = 200;
  dut->pc_plus4 = 300;
  dut->eval();
  EXPECT_EQ(dut->dout, 300);
}
