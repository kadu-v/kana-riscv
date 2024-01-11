#include <stdint.h>
#include <stdio.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include <algorithm>  // std::replace
#include <fstream>
#include <iostream>
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
  void set_csr_reg(uint32_t addr, uint32_t val) {
    dut_->riscv_top__DOT__csr_regs__DOT__regs[addr] = val;
  }
  uint32_t get_csr_reg(uint32_t addr) {
    return dut_->riscv_top__DOT__csr_regs__DOT__regs[addr];
  }
  void set_ram(uint32_t addr, uint32_t inst) {
    dut_->riscv_top__DOT__ram__DOT__mem[addr + 0] =
        ((inst & 0b00000000000000000000000011111111) >> 0);
    dut_->riscv_top__DOT__ram__DOT__mem[addr + 1] =
        ((inst & 0b00000000000000001111111100000000) >> 8);
    dut_->riscv_top__DOT__ram__DOT__mem[addr + 2] =
        ((inst & 0b00000000111111110000000000000000) >> 16);
    dut_->riscv_top__DOT__ram__DOT__mem[addr + 3] =
        ((inst & 0b11111111000000000000000000000000) >> 24);
  }
  uint32_t get_ram(uint32_t addr) {
    uint32_t inst0 = dut_->riscv_top__DOT__ram__DOT__mem[addr + 0];
    uint32_t inst1 = (dut_->riscv_top__DOT__ram__DOT__mem[addr + 1] << 8);
    uint32_t inst2 = (dut_->riscv_top__DOT__ram__DOT__mem[addr + 2] << 16);
    uint32_t inst3 = (dut_->riscv_top__DOT__ram__DOT__mem[addr + 3] << 24);
    return inst3 + inst2 + inst1 + inst0;
  }
  void set_byte_ram(uint32_t addr, char inst) {
    dut_->riscv_top__DOT__ram__DOT__mem[addr] = inst;
  }
  void eval() {
    dut_->eval();
    tfp_->dump(cnt_);
    cnt_++;
  }
  void start() {
    std::replace(test_name_.begin(), test_name_.end(), ' ', '_');
    Verilated::traceEverOn(true);
    dut_->trace(tfp_, 1000);
    std::cout << std::format("./vcds/{}.vcd", test_name_) << std::endl;
    tfp_->open(std::format("./vcds/{}.vcd", test_name_).c_str());
  }
  void finish() {
    dut_->final();
    tfp_->close();
  }
  int set_insts_from_file(std::string filename) {
    std::ifstream ifs(filename, std::ios::binary);

    ifs.seekg(0, std::ios::end);
    long long int size = ifs.tellg();
    ifs.seekg(0);

    if (size < 0) {
      std::cout << "Not find file: " << filename << std::endl;
      return 0;
    }

    char* data = new char[size];
    ifs.read(data, size);

    for (int i = 0; i < size; i++) {
      this->set_byte_ram(i, data[i]);
    }
    std::cout << "size: " << size << std::endl;
    std::cout << "Completely set instruction to ram from " << filename
              << std::endl;
    delete data;
    return 1;
  }
};