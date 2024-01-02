#include <stdint.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include <string>

#include "Vriscv_top.h"

class TopTester {
 public:
  Vriscv_top* dut_;
  VerilatedVcdC* tfp_;
  int cnt_;
  std::string test_name_;
  TopTester(std::string test_name)
      : dut_(new Vriscv_top()),
        tfp_(new VerilatedVcdC),
        test_name_(test_name),
        cnt_(0) {}
  void set_reg(uint32_t addr, uint32_t val) {
    dut_->riscv_top__DOT__regs__DOT__regs[addr] = val;
  }
  uint32_t get_reg(uint32_t addr) {
    return dut_->riscv_top__DOT__regs__DOT__regs[addr];
  }
  void set_ram(uint32_t addr, uint32_t inst) {
    dut_->riscv_top__DOT__ram__DOT__mem[addr] = inst;
  }
  uint32_t get_ram(uint32_t addr) {
    return dut_->riscv_top__DOT__ram__DOT__mem[addr];
  }
  void eval() {
    dut_->eval();
    tfp_->dump(cnt_);
    cnt_++;
  }
  void start() {
    Verilated::traceEverOn(true);
    dut_->trace(tfp_, 100);
    tfp_->open(std::format("{}.vcd", test_name_).c_str());
  }
  void finish() {
    dut_->final();
    tfp_->close();
  }
};