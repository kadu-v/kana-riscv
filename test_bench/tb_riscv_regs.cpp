#include <gtest/gtest.h>
#include <stdint.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include <format>
#include <iostream>

#include "Vriscv_regs.h"
#include "assert.h"
#include "riscv_defs.h"

TEST(REGS, PEEK_POKE) {
  // Instantiate DUT
  Vriscv_regs* dut = new Vriscv_regs();

  // initiazlie decoder
  int time_counter = 0;
  dut->clk = 0;
  dut->write_en = 0;
  dut->read_addr1 = 0;
  dut->read_addr2 = 0;
  dut->data = 0;
  dut->write_addr = 0;

  while (time_counter < 100) {
    dut->eval();
    time_counter++;
  }

  //
  int data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int addr[10] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

  for (int i = 0; i < 10; i++) {
    dut->clk = 0;  // Low
    dut->eval();
    dut->read_addr1 = addr[i];
    dut->write_addr = addr[i];
    dut->data = data[i];
    dut->write_en = 1;
    dut->clk = 1;  // High
    dut->eval();
    EXPECT_EQ(dut->read_data1, data[i]);
  }
}
