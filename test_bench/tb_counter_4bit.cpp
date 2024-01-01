#include <verilated.h>

#include <iostream>

#include "Vcounter_4bit.h"

int time_counter = 0;

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);

  // Instantiate DUT
  Vcounter_4bit* dut = new Vcounter_4bit();

  // Format
  dut->reset_n = 0;
  dut->clk = 0;
  dut->en = 0;

  // Reset Time
  while (time_counter < 100) {
    dut->eval();
    time_counter++;
  }
  // Release reset
  dut->reset_n = 1;

  int cycle = 0;
  while (time_counter < 500) {
    if ((time_counter % 5) == 0) {
      dut->clk = !dut->clk;  // Toggle clock
    }
    if ((time_counter % 10) == 0) {
      // Cycle Count
      cycle++;
    }

    if (cycle % 5 == 0) {
      dut->en = 1;  // Assert En
    } else {
      dut->en = 0;  // Deassert En
    }

    // Evaluate DUT
    dut->eval();

    time_counter++;
  }

  // std::cout << "Final Counter Value = " << dut->cnt << '\n';
  printf("Final Counter Value = %d\n", dut->cnt);

  dut->final();
}