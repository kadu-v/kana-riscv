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

  // slt
  dut->inst = slt(0b01 /* rs2 */, 0b10 /* rs1 */, 0b11 /* rd */);
  dut->eval();
  std::vector<uint32_t> slt_expected{
      1 /* valid */,  3 /* ALU_SLT */,  1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      1 /* WB_ALU */, 1 /* RF_WRITE */, 0 /* MEM_X */,   0 /* PC_PLUS4 */
  };
  test_decode("slt", dut, slt_expected);

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
      1 /* valid */, 1 /* ALU_ADD */,  1 /* OP1_RS1 */, 4 /* OP2_IMJ */,
      3 /* WB_PC */, 1 /* RF_WRITE */, 0 /* MEM_X */,   1 /* PC_J_TARGET */
  };
  test_decode("jal", dut, jal_expected);

  /* ---------------------------------------------------------------------
  B type
  ---------------------------------------------------------------------*/
  // beq
  dut->inst = beq(0b0 /* rs1 */, 0b01 /* rs2 */, 0b10 /* imm */);
  dut->eval();
  std::vector<uint32_t> beq_expected{
      1 /* valid */, 0 /* ALU_X*/, 1 /* OP1_RS1 */, 1 /* OP2_RS2 */,
      0 /* WB_X */,  0 /* RF_X */, 0 /* MEM_X */,   2 /* PC_B_TARGET */
  };
  test_decode("beq", dut, beq_expected);
}