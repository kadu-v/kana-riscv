#include <verilated.h>

#include <format>
#include <iostream>
#include <vector>

#include "Vriscv_decoder.h"
#include "assert.h"
#include "riscv_defs.h"

void test_decode(std::string inst_name, Vriscv_decoder* dut,
                 std::vector<uint32_t> expected) {
  assert_eq(std::format("[{}] check validation", inst_name), dut->invalid_o,
            expected[0]);
  assert_eq(std::format("[{}] check exec_fun", inst_name), dut->exec_fun,
            expected[1]);
  assert_eq(std::format("[{}] check op1_sel", inst_name), dut->op1_sel,
            expected[2]);
  assert_eq(std::format("[{}] check op2_sel", inst_name), dut->op2_sel,
            expected[3]);
  assert_eq(std::format("[{}] check wb_sel", inst_name), dut->wb_sel,
            expected[4]);
  assert_eq(std::format("[{}] check rf_wen", inst_name), dut->rf_wen,
            expected[5]);
  assert_eq(std::format("[{}] check mem_wen", inst_name), dut->mem_wen,
            expected[6]);
  assert_eq(std::format("[{}] check pc_sel", inst_name), dut->pc_sel,
            expected[7]);
}

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);

  // Instantiate DUT
  Vriscv_decoder* dut = new Vriscv_decoder();

  // initiazlie decoder
  /* do nothing */

  // set input instractions
  // invalid instruction
  dut->inst = 0;
  dut->eval();
  std::vector<uint32_t> invalid_expected{
      0 /* invalid */, 0 /* ALU_X */, 0 /* OP1_X */, 0 /* OP2_X */,
      0 /* WB_X */,    0 /* RF_X */,  0 /* MEM_X */, 0 /* PC_PLUS4 */
  };
  test_decode("invalid", dut, invalid_expected);

  /* ---------------------------------------------------------------------
    R type
   ---------------------------------------------------------------------*/
  // add
  dut->inst = add(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> add_expected{
      1 /* valid */,  1 /* ALU_ADD */,  1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("add", dut, add_expected);

  // sub
  dut->inst = sub(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> sub_expected{
      1 /* valid */,  2 /* ALU_SUB */,  1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("sub", dut, sub_expected);

  // sll
  dut->inst = sll(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> sll_expected{
      1 /* valid */,  3 /* ALU_SLL */,  1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("sll", dut, sll_expected);

  // slt
  dut->inst = slt(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> slt_expected{
      1 /* valid */,  4 /* ALU_SLT */,  1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("slt", dut, slt_expected);

  // sltu
  dut->inst = sltu(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> sltu_expected{
      1 /* valid */,  5 /* ALU_SLTU */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("sltu", dut, sltu_expected);

  // xor
  dut->inst = ixor(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> xor_expected{
      1 /* valid */,  6 /* ALU_SLTU */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("xor", dut, xor_expected);

  // srl
  dut->inst = srl(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> srl_expected{
      1 /* valid */,  7 /* ALU_SRL */,  1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("srl", dut, srl_expected);

  // sra
  dut->inst = sra(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> sra_expected{
      1 /* valid */,  8 /* ALU_SRA */,  1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("sra", dut, sra_expected);

  // or
  dut->inst = ior(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> or_expected{
      1 /* valid */,  9 /* ALU_OR */,   1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("or", dut, or_expected);

  // slt
  dut->inst = iand(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> and_expected{
      1 /* valid */,  10 /* ALU_AND */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("and", dut, and_expected);

  /* ---------------------------------------------------------------------
    I type
   ---------------------------------------------------------------------*/
  // addi
  dut->inst = addi(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> addi_expected{
      1 /* valid */,  1 /* ALU_ADD */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("addi", dut, addi_expected);

  // slti
  dut->inst = slti(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> slti_expected{
      1 /* valid */,  4 /* ALU_SLT */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("slti", dut, slti_expected);

  // sltiu
  dut->inst = sltiu(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> sltiu_expected{
      1 /* valid */,  5 /* ALU_SLTU */, 1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("sltiu", dut, sltiu_expected);

  // xori
  dut->inst = xori(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> xori_expected{
      1 /* valid */,  6 /* ALU_XOR */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("xori", dut, xori_expected);

  // ori
  dut->inst = ori(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> ori_expected{
      1 /* valid */,  9 /* ALU_OR */,   1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("ori", dut, ori_expected);

  // andi
  dut->inst = andi(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> andi_expected{
      1 /* valid */,  10 /* ALU_AND */, 1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("ori", dut, andi_expected);

  // slli
  dut->inst = slli(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> slli_expected{
      1 /* valid */,  3 /* ALU_SLL */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("slli", dut, slli_expected);

  // srli
  dut->inst = srli(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> srli_expected{
      1 /* valid */,  7 /* ALU_SRL */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("srli", dut, srli_expected);

  // srai
  dut->inst = srai(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> srai_expected{
      1 /* valid */,  8 /* ALU_SRA */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("srai", dut, srai_expected);

  // lw
  dut->inst = lw(0b01 /* imm */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> lw_expected{
      1 /* valid */,  1 /* ALU_ADD */,  1 /* OP1_RS1 */, 2 /* OP2_IMI */,
      2 /* WB_MEM */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("lw", dut, lw_expected);

  /* ---------------------------------------------------------------------
  S type
 ---------------------------------------------------------------------*/
  // sw
  dut->inst = sw(0b01 /* imm */, 0b01 /* rs1 */, 0b10 /* rd */);
  dut->eval();
  std::vector<uint32_t> sw_expected{
      1 /* valid */, 1 /* ALU_ADD */, 1 /* OP1_RS1 */,   3 /* OP2_IMS */,
      0 /* WB_X */,  0 /* RF_X */,    1 /* MEM_WRITE */, 0 /* PC_PLUS4 */
  };
  test_decode("sw", dut, sw_expected);

  /* ---------------------------------------------------------------------
  J type
  ---------------------------------------------------------------------*/
  // jal
  dut->inst = jal(0b0 /* rd */, 0b01 /* imm */);
  dut->eval();
  std::vector<uint32_t> jal_expected{
      1 /* valid */, 1 /* ALU_ADD */,  2 /* OP1_PC */, 4 /* OP2_IMJ */,
      3 /* WB_PC */, 1 /* RF_WRITE */, 0 /* MEM_X */,  1 /* PC_ALU */
  };
  test_decode("jal", dut, jal_expected);

  /* ---------------------------------------------------------------------
  B type
  ---------------------------------------------------------------------*/
  // beq
  dut->inst = beq(0b0 /* rs1 */, 0b01 /* rs2 */, 0b10 /* imm */);
  dut->eval();
  std::vector<uint32_t> beq_expected{
      1 /* valid */, 11 /* ALU_BEQ */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      0 /* WB_X */,  0 /* RF_X */,     0 /* MEM_X */,   2 /* PC_B_TARGET */
  };
  test_decode("beq", dut, beq_expected);

  // bne
  dut->inst = bne(0b0 /* rs1 */, 0b01 /* rs2 */, 0b10 /* imm */);
  dut->eval();
  std::vector<uint32_t> bne_expected{
      1 /* valid */, 12 /* ALU_BNE */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      0 /* WB_X */,  0 /* RF_X */,     0 /* MEM_X */,   2 /* PC_B_TARGET */
  };
  test_decode("bne", dut, bne_expected);

  // blt
  dut->inst = blt(0b0 /* rs1 */, 0b01 /* rs2 */, 0b10 /* imm */);
  dut->eval();
  std::vector<uint32_t> blt_expected{
      1 /* valid */, 13 /* ALU_BLT */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      0 /* WB_X */,  0 /* RF_X */,     0 /* MEM_X */,   2 /* PC_B_TARGET */
  };
  test_decode("blt", dut, blt_expected);

  // bge
  dut->inst = bge(0b0 /* rs1 */, 0b01 /* rs2 */, 0b10 /* imm */);
  dut->eval();
  std::vector<uint32_t> bge_expected{
      1 /* valid */, 14 /* ALU_BGE */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      0 /* WB_X */,  0 /* RF_X */,     0 /* MEM_X */,   2 /* PC_B_TARGET */
  };
  test_decode("bge", dut, bge_expected);

  // bltu
  dut->inst = bltu(0b0 /* rs1 */, 0b01 /* rs2 */, 0b10 /* imm */);
  dut->eval();
  std::vector<uint32_t> bltu_expected{
      1 /* valid */, 15 /* ALU_BLTU */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      0 /* WB_X */,  0 /* RF_X */,      0 /* MEM_X */,   2 /* PC_B_TARGET */
  };
  test_decode("bltu", dut, bltu_expected);

  // bgeu
  dut->inst = bgeu(0b0 /* rs1 */, 0b01 /* rs2 */, 0b10 /* imm */);
  dut->eval();
  std::vector<uint32_t> bgeu_expected{
      1 /* valid */, 16 /* ALU_BGEU */, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      0 /* WB_X */,  0 /* RF_X */,      0 /* MEM_X */,   2 /* PC_B_TARGET */
  };
  test_decode("bgeu", dut, bgeu_expected);

  /* ---------------------------------------------------------------------
  U type
  ---------------------------------------------------------------------*/
  // lui
  dut->inst = lui(0b0 /* rd */, 0b01 /* imm */);
  dut->eval();
  std::vector<uint32_t> lui_expected{
      1 /* valid */,  1 /* ALU_ADD */,  0 /* OP1_X */, 5 /* OP2_IMU */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */, 0 /* PC_PLUS4 */
  };
  test_decode("lui", dut, lui_expected);

  // auipc
  dut->inst = auipc(0b1 /* rd */, 0b01 /* rs2 */);
  dut->eval();
  std::vector<uint32_t> auipc_expected{
      1 /* valid */,  1 /* ALU_ADD */,  2 /* OP1_PC */, 5 /* OP2_IMU */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,  0 /* PC_PLUS4 */
  };
  test_decode("auipc", dut, auipc_expected);
}