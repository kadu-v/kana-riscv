#include <gtest/gtest.h>
#include <stdint.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#include <format>
#include <iostream>
#include <vector>

#include "Vriscv_decoder.h"
#include "assert.h"
#include "riscv_defs.h"

void test_decode(Vriscv_decoder* dut, std::vector<uint32_t> expected) {
  EXPECT_EQ(dut->invalid_o, expected[0]);
  EXPECT_EQ(dut->exec_fun, expected[1]);
  EXPECT_EQ(dut->op1_sel, expected[2]);
  EXPECT_EQ(dut->op2_sel, expected[3]);
  EXPECT_EQ(dut->wb_sel, expected[4]);
  EXPECT_EQ(dut->rf_wen, expected[5]);
  EXPECT_EQ(dut->mem_wen, expected[6]);
  EXPECT_EQ(dut->pc_sel, expected[7]);
  EXPECT_EQ(dut->rs2_mask_sel, expected[8]);
  EXPECT_EQ(dut->ram_mask_sel, expected[9]);
  EXPECT_EQ(dut->csr_fun, expected[10]);
  EXPECT_EQ(dut->csr_wen, expected[11]);
}

TEST(DECODER, INVALID) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  dut->inst = 0;
  dut->eval();
  std::vector<uint32_t> invalid_expected{
      0 /* invalid */, 0 /* ALU_X */,  0 /* OP1_X */, 0 /* OP2_X */,
      0 /* WB_X */,    0 /* RF_X */,   0 /* MEM_X */, 0 /* PC_PLUS4 */,
      0 /* MASK_X */,  0 /* MASK_X */, 0 /* CSR_X */, 0 /* CSR_WEN_X */};
  test_decode(dut, invalid_expected);
}

/* ---------------------------------------------------------------------
  R type
 ---------------------------------------------------------------------*/
TEST(DECODER, ADD) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // add
  dut->inst = add(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> add_expected{
      1 /* valid */,  1 /* ALU_ADD */,  1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, add_expected);
}

TEST(DECODER, SUB) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // sub
  dut->inst = sub(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> sub_expected{
      1 /* valid */,  2 /* ALU_SUB */,  1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, sub_expected);
}

TEST(DECODER, SLL) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // sll
  dut->inst = sll(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> sll_expected{
      1 /* valid */,  3 /* ALU_SLL */,  1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, sll_expected);
}

TEST(DECODER, SLT) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // slt
  dut->inst = slt(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> slt_expected{
      1 /* valid */,  4 /* ALU_SLT */,  1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, slt_expected);
}

TEST(DECODER, SLUTU) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // sltu
  dut->inst = sltu(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> sltu_expected{
      1 /* valid */,  5 /* ALU_SLTU */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, sltu_expected);
}

TEST(DECODER, XOR) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // xor
  dut->inst = ixor(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> xor_expected{
      1 /* valid */,  6 /* ALU_SLTU */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, xor_expected);
}

TEST(DECODER, SRL) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // srl
  dut->inst = srl(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> srl_expected{
      1 /* valid */,  7 /* ALU_SRL */,  1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, srl_expected);
}

TEST(DECODER, SRA) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // sra
  dut->inst = sra(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> sra_expected{
      1 /* valid */,  8 /* ALU_SRA */,  1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, sra_expected);
}

TEST(DECODER, OR) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // or
  dut->inst = ior(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> or_expected{
      1 /* valid */,  9 /* ALU_OR */,   1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, or_expected);
}

TEST(DECODER, AND) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // slt
  dut->inst = iand(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> and_expected{
      1 /* valid */,  10 /* ALU_AND */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, and_expected);
}

/* ---------------------------------------------------------------------
  I type
 ---------------------------------------------------------------------*/

TEST(DECODER, JALR) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // jalr
  dut->inst = jalr(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> jalr_expected{
      1 /* valid */,  17 /* ALU_JALR */, 1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      3 /* WB_PC */,  1 /* RF_WRITE */,  0 /* MEM_X */,   1 /* PC_ALU */,
      0 /* MASK_X */, 0 /* MASK_X */,    0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, jalr_expected);
}

TEST(DECODER, ADDI) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // addi
  dut->inst = addi(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> addi_expected{
      1 /* valid */,  1 /* ALU_ADD */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, addi_expected);
}

TEST(DECODER, SLTI) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // slti
  dut->inst = slti(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> slti_expected{
      1 /* valid */,  4 /* ALU_SLT */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, slti_expected);
}

TEST(DECODER, SLTIU) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // sltiu
  dut->inst = sltiu(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> sltiu_expected{
      1 /* valid */,  5 /* ALU_SLTU */, 1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, sltiu_expected);
}

TEST(DECODER, XORI) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // xori
  dut->inst = xori(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> xori_expected{
      1 /* valid */,  6 /* ALU_XOR */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, xori_expected);
}

TEST(DECODER, ORI) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // ori
  dut->inst = ori(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> ori_expected{
      1 /* valid */,  9 /* ALU_OR */,   1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, ori_expected);
}

TEST(DECODER, ANDI) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // andi
  dut->inst = andi(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> andi_expected{
      1 /* valid */,  10 /* ALU_AND */, 1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, andi_expected);
}

TEST(DECODER, SLLI) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // slli
  dut->inst = slli(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> slli_expected{
      1 /* valid */,  3 /* ALU_SLL */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, slli_expected);
}

TEST(DECODER, SRLI) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // srli
  dut->inst = srli(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> srli_expected{
      1 /* valid */,  7 /* ALU_SRL */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, srli_expected);
}

TEST(DECODER, SRAI) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // srai
  dut->inst = srai(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> srai_expected{
      1 /* valid */,  8 /* ALU_SRA */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, srai_expected);
}

TEST(DECODER, LB) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // lb
  dut->inst = lb(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> lb_expected{
      1 /* valid */,  1 /* ALU_ADD */,     1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      2 /* WB_MEM */, 1 /* RF_WRITE */,    0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 3 /* MASK_B_SEXT */, 0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, lb_expected);
}

TEST(DECODER, LH) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // lh
  dut->inst = lh(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> lh_expected{
      1 /* valid */,  1 /* ALU_ADD */,     1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      2 /* WB_MEM */, 1 /* RF_WRITE */,    0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 4 /* MASK_H_SEXT */, 0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, lh_expected);
}

TEST(DECODER, LW) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // lw
  dut->inst = lw(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> lw_expected{
      1 /* valid */,  1 /* ALU_ADD */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      2 /* WB_MEM */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, lw_expected);
}

TEST(DECODER, LBU) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // lbu
  dut->inst = lbu(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> lbu_expected{
      1 /* valid */,  1 /* ALU_ADD */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      2 /* WB_MEM */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 1 /* MASK_B */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, lbu_expected);
}

TEST(DECODER, LHU) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // lhu
  dut->inst = lhu(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> lhu_expected{
      1 /* valid */,  1 /* ALU_ADD */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      2 /* WB_MEM */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 2 /* MASK_H */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, lhu_expected);
}

TEST(DECODER, CSRRW) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // csrrw
  dut->inst = csrrw(0b01 /* rd */, 0b10 /* csr */, 0b11 /* rs1 */);
  dut->eval();
  std::vector<uint32_t> csrrw_expected{
      1 /* valid */,  0 /* ALU_X */,    1 /* OP1_RS1 */, 0 /* OP2_X */,
      4 /* WB_CSR */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   1 /* CSR_W */,   1 /* CSR_WRITE */};
  test_decode(dut, csrrw_expected);
}

TEST(DECODER, CSRRWI) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // csrrwi
  dut->inst = csrrwi(0b01 /* rd */, 0b10 /* csr */, 0b11 /* rs1 */);
  dut->eval();
  std::vector<uint32_t> csrrwi_expected{
      1 /* valid */,  0 /* ALU_X */,    3 /* OP1_IMZ */, 0 /* OP2_X */,
      4 /* WB_CSR */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   1 /* CSR_W */,   1 /* CSR_WRITE */};
  test_decode(dut, csrrwi_expected);
}

TEST(DECODER, CSRRS) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // csrrs
  dut->inst = csrrs(0b01 /* rd */, 0b10 /* csr */, 0b11 /* rs1 */);
  dut->eval();
  std::vector<uint32_t> csrrs_expected{
      1 /* valid */,  0 /* ALU_X */,    1 /* OP1_RS1 */, 0 /* OP2_X */,
      4 /* WB_CSR */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   2 /* CSR_S */,   1 /* CSR_WRITE */};
  test_decode(dut, csrrs_expected);
}

TEST(DECODER, CSRRSI) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // csrrsi
  dut->inst = csrrsi(0b01 /* rd */, 0b10 /* csr */, 0b11 /* rs1 */);
  dut->eval();
  std::vector<uint32_t> csrrsi_expected{
      1 /* valid */,  0 /* ALU_X */,    3 /* OP1_IMZ */, 0 /* OP2_X */,
      4 /* WB_CSR */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   2 /* CSR_S */,   1 /* CSR_WRITE */};
  test_decode(dut, csrrsi_expected);
}

TEST(DECODER, CSRRC) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // csrrc
  dut->inst = csrrc(0b01 /* rd */, 0b10 /* csr */, 0b11 /* rs1 */);
  dut->eval();
  std::vector<uint32_t> csrrc_expected{
      1 /* valid */,  0 /* ALU_X */,    1 /* OP1_RS1 */, 0 /* OP2_X */,
      4 /* WB_CSR */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   3 /* CSR_C */,   1 /* CSR_WRITE */};
  test_decode(dut, csrrc_expected);
}

TEST(DECODER, ECALL) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // csrrci
  dut->inst = csrrci(0b01 /* rd */, 0b10 /* csr */, 0b11 /* rs1 */);
  dut->eval();
  std::vector<uint32_t> csrrci_expected{
      1 /* valid */,  0 /* ALU_X */,    3 /* OP1_IMZ */, 0 /* OP2_X */,
      4 /* WB_CSR */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   3 /* CSR_C */,   1 /* CSR_WRITE */};
  test_decode(dut, csrrci_expected);
}

TEST(DECODER, CSRRCI) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // ecall
  dut->inst = ecall();
  dut->eval();
  std::vector<uint32_t> ecall_expected{
      1 /* valid */,  0 /* ALU_X */,  0 /* OP1_X */, 0 /* OP2_X */,
      0 /* WB_X */,   0 /* RF_X */,   0 /* MEM_X */, 3 /* PC_ECALL_TARGET */,
      0 /* MASK_X */, 0 /* MASK_X */, 4 /* CSR_E */, 1 /* CSR_WRITE */};
  test_decode(dut, ecall_expected);
}

/* ---------------------------------------------------------------------
S type
---------------------------------------------------------------------*/

TEST(DECODER, SB) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // sb
  dut->inst = sb(0b01 /* imm */, 0b01 /* rs1 */, 0b10 /* rd */);
  dut->eval();
  std::vector<uint32_t> sb_expected{
      1 /* valid */,  1 /* ALU_ADD */, 1 /* OP1_RS1 */,   3 /* OP2_IMS */,
      0 /* WB_X */,   0 /* RF_X */,    1 /* MEM_WRITE */, 0 /* PC_PLUS4 */,
      1 /* MASK_B */, 0 /* MASK_X */,  0 /* CSR_X */,     0 /* CSR_WEN_X */};
  test_decode(dut, sb_expected);
}

TEST(DECODER, SH) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // sh
  dut->inst = sh(0b01 /* imm */, 0b01 /* rs1 */, 0b10 /* rd */);
  dut->eval();
  std::vector<uint32_t> sh_expected{
      1 /* valid */,  1 /* ALU_ADD */, 1 /* OP1_RS1 */,   3 /* OP2_IMS */,
      0 /* WB_X */,   0 /* RF_X */,    1 /* MEM_WRITE */, 0 /* PC_PLUS4 */,
      2 /* MASK_H */, 0 /* MASK_X */,  0 /* CSR_X */,     0 /* CSR_WEN_X */};
  test_decode(dut, sh_expected);
}

TEST(DECODER, SW) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // sw
  dut->inst = sw(0b01 /* imm */, 0b01 /* rs1 */, 0b10 /* rd */);
  dut->eval();
  std::vector<uint32_t> sw_expected{
      1 /* valid */,  1 /* ALU_ADD */, 1 /* OP1_RS1 */,   3 /* OP2_IMS */,
      0 /* WB_X */,   0 /* RF_X */,    1 /* MEM_WRITE */, 0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,  0 /* CSR_X */,     0 /* CSR_WEN_X */};
  test_decode(dut, sw_expected);
}

/* ---------------------------------------------------------------------
J type
---------------------------------------------------------------------*/
TEST(DECODER, JAL) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // jal
  dut->inst = jal(0b0 /* rd */, 0b01 /* imm */);
  dut->eval();
  std::vector<uint32_t> jal_expected{
      1 /* valid */,  1 /* ALU_ADD */,  2 /* OP1_PC */, 4 /* OP2_IMJ */,
      3 /* WB_PC */,  1 /* RF_WRITE */, 0 /* MEM_X */,  1 /* PC_ALU */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,  0 /* CSR_WEN_X */};
  test_decode(dut, jal_expected);
}

/* ---------------------------------------------------------------------
B type
---------------------------------------------------------------------*/
TEST(DECODER, BEQ) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // beq
  dut->inst = beq(0b0 /* rs1 */, 0b01 /* rs2 */, 0b10 /* imm */);
  dut->eval();
  std::vector<uint32_t> beq_expected{
      1 /* valid */,  11 /* ALU_BEQ */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      0 /* WB_X */,   0 /* RF_X */,     0 /* MEM_X */,   2 /* PC_B_TARGET */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, beq_expected);
}

TEST(DECODER, BNE) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // bne
  dut->inst = bne(0b0 /* rs1 */, 0b01 /* rs2 */, 0b10 /* imm */);
  dut->eval();
  std::vector<uint32_t> bne_expected{
      1 /* valid */,  12 /* ALU_BNE */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      0 /* WB_X */,   0 /* RF_X */,     0 /* MEM_X */,   2 /* PC_B_TARGET */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, bne_expected);
}

TEST(DECODER, BLT) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // blt
  dut->inst = blt(0b0 /* rs1 */, 0b01 /* rs2 */, 0b10 /* imm */);
  dut->eval();
  std::vector<uint32_t> blt_expected{
      1 /* valid */,  13 /* ALU_BLT */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      0 /* WB_X */,   0 /* RF_X */,     0 /* MEM_X */,   2 /* PC_B_TARGET */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, blt_expected);
}

TEST(DECODER, BGE) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // bge
  dut->inst = bge(0b0 /* rs1 */, 0b01 /* rs2 */, 0b10 /* imm */);
  dut->eval();
  std::vector<uint32_t> bge_expected{
      1 /* valid */,  14 /* ALU_BGE */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      0 /* WB_X */,   0 /* RF_X */,     0 /* MEM_X */,   2 /* PC_B_TARGET */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, bge_expected);
}

TEST(DECODER, BLTU) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // bltu
  dut->inst = bltu(0b0 /* rs1 */, 0b01 /* rs2 */, 0b10 /* imm */);
  dut->eval();
  std::vector<uint32_t> bltu_expected{
      1 /* valid */,  15 /* ALU_BLTU */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      0 /* WB_X */,   0 /* RF_X */,      0 /* MEM_X */,   2 /* PC_B_TARGET */,
      0 /* MASK_X */, 0 /* MASK_X */,    0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, bltu_expected);
}

TEST(DECODER, BGEU) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // bgeu
  dut->inst = bgeu(0b0 /* rs1 */, 0b01 /* rs2 */, 0b10 /* imm */);
  dut->eval();
  std::vector<uint32_t> bgeu_expected{
      1 /* valid */,  16 /* ALU_BGEU */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      0 /* WB_X */,   0 /* RF_X */,      0 /* MEM_X */,   2 /* PC_B_TARGET */,
      0 /* MASK_X */, 0 /* MASK_X */,    0 /* CSR_X */,   0 /* CSR_WEN_X */};
  test_decode(dut, bgeu_expected);
}

/* ---------------------------------------------------------------------
U type
---------------------------------------------------------------------*/
TEST(DECODER, LUI) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // lui
  dut->inst = lui(0b0 /* rd */, 0b01 /* imm */);
  dut->eval();
  std::vector<uint32_t> lui_expected{
      1 /* valid */,  1 /* ALU_ADD */,  0 /* OP1_X */, 5 /* OP2_IMU */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */, 0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */, 0 /* CSR_WEN_X */};
  test_decode(dut, lui_expected);
}

TEST(DECODER, AUIPC) {
  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // auipc
  dut->inst = auipc(0b1 /* rd */, 0b01 /* rs2 */);
  dut->eval();
  std::vector<uint32_t> auipc_expected{
      1 /* valid */,  1 /* ALU_ADD */,  2 /* OP1_PC */, 5 /* OP2_IMU */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,  0 /* PC_PLUS4 */,
      0 /* MASK_X */, 0 /* MASK_X */,   0 /* CSR_X */,  0 /* CSR_WEN_X */};
  test_decode(dut, auipc_expected);
}
