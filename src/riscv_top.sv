`include "riscv_decoder.sv"
`include "riscv_alu.sv"
`include "riscv_regs.sv"
`include "riscv_constants.sv"
`include "riscv_mux1.sv"
`include "riscv_mux2.sv"
`include "riscv_mask1.sv"
`include "riscv_mask2.sv"
`include "riscv_csr_regs.sv"
`include "riscv_csr_alu.sv"

module riscv_top (
    input logic clk,
    input logic x_reset
);

  /* pc */
  logic    [31:0] pc_plus4;
  logic    [31:0] pc_out;

  /* ram */
  logic    [31:0] inst;
  logic    [31:0] ram_dout;

  /* decoder */
  logic           invalid_o;
  EXEC_FUN        exec_fun;
  OP1_SEL         op1_sel;
  OP2_SEL         op2_sel;
  WB_SEL          wb_sel;
  RF_WEN          rf_wen;
  MEM_WEN         mem_wen;
  PC_SEL          pc_sel;
  MASK_SEL        rs2_mask_sel;
  MASK_SEL        ram_mask_sel;
  CSR_FUN         csr_fun;
  CSR_WEN         csr_wen;

  /* register files */
  logic    [ 4:0] rs1_addr;
  logic    [ 4:0] rs2_addr;
  logic    [ 4:0] rd_addr;
  assign rs1_addr = inst[19:15];
  assign rs2_addr = inst[24:20];
  assign rd_addr  = inst[11:7];

  /* extend */
  logic [31:0] imm_i_sext;
  logic [31:0] imm_s_sext;
  logic [31:0] imm_j_sext;
  logic [31:0] imm_b_sext;
  logic [31:0] imm_u_sext;
  logic [31:0] imm_z_uext;

  /* mask1 */
  logic [31:0] mask1_out;

  /* mask2 */
  logic [31:0] mask2_out;

  /* mux1 */
  logic [31:0] mux1_dout;

  /* mux2 */
  logic [31:0] mux2_dout;

  /* alu */
  logic [31:0] rs1_data;
  logic [31:0] rs2_data;
  logic [31:0] alu_dout;
  logic        br_flag;

  /* csr_regs */
  /* input */
  logic [31:0] csr_addr;
  assign csr_addr = {{20{1'b0}}, inst[31:20]};
  /* output */
  logic [31:0] csr_dout;

  /* csr_alu */
  logic [31:0] csr_alu_out;

  /* wb mux */
  logic [31:0] wb_mux_dout;

  /* pc */
  riscv_pc pc (
      /* input */
      .clk       (clk),
      .x_reset   (x_reset),
      .pc_sel    (pc_sel),
      .alu_out   (alu_dout),
      .imm_b_sext(imm_b_sext),
      .br_flag   (br_flag),
      .mtvec_addr(csr_dout),
      /* output */
      .pc_plus4  (pc_plus4),
      .pc_out    (pc_out)
  );

  /* ram */
  riscv_ram ram (
      .clk         (clk),
      /* port for instruction */
      .pc          (pc_out),
      .inst        (inst),
      /* port for data */
      .addr        (alu_dout),
      .write_en    (mem_wen),
      .wdata       (mask1_out),
      .ram_mask_sel(rs2_mask_sel),
      .dout        (ram_dout)
  );

  /* decoder */
  riscv_decoder decoder (
      /* input */
      .inst        (inst),
      /* output */
      .invalid_o   (invalid_o),
      .exec_fun    (exec_fun),
      .op1_sel     (op1_sel),
      .op2_sel     (op2_sel),
      .wb_sel      (wb_sel),
      .rf_wen      (rf_wen),
      .mem_wen     (mem_wen),
      .pc_sel      (pc_sel),
      .rs2_mask_sel(rs2_mask_sel),
      .ram_mask_sel(ram_mask_sel),
      .csr_fun     (csr_fun),
      .csr_wen     (csr_wen)
  );

  /* register files*/
  riscv_regs regs (
      /* input */
      .clk       (clk),
      .write_en  (rf_wen),
      .read_addr1(rs1_addr),
      .read_addr2(rs2_addr),
      .data      (wb_mux_dout),
      .write_addr(rd_addr),
      /* output */
      .read_data1(rs1_data),
      .read_data2(rs2_data)
  );

  /* extend */
  riscv_extend extend (
      /* input */
      .inst(inst),
      /* output */
      .imm_i_sext(imm_i_sext),
      .imm_s_sext(imm_s_sext),
      .imm_j_sext(imm_j_sext),
      .imm_b_sext(imm_b_sext),
      .imm_u_sext(imm_u_sext),
      .imm_z_uext(imm_z_uext)
  );

  /* mask1 */
  riscv_mask1 mask1 (
      /* input */
      .rs2_data(rs2_data),
      .rs2_mask_sel(rs2_mask_sel),
      /* output */
      .mask_out(mask1_out)
  );

  /* mask2 */
  riscv_mask2 mask2 (
      /* input */
      .ram_data(ram_dout),
      .ram_mask_sel(ram_mask_sel),
      /* output */
      .mask_out(mask2_out)
  );

  /* mux2 */
  riscv_mux1 mux1 (
      /* input */
      .op1_sel   (op1_sel),
      .rs1_data  (rs1_data),
      .pc        (pc_out),
      .imm_z_uext(imm_z_uext),
      /* output */
      .dout      (mux1_dout)
  );

  /* mux2 */
  riscv_mux2 mux2 (
      /* input */
      .op2_sel   (op2_sel),
      .rs2_data  (rs2_data),
      .imm_i_sext(imm_i_sext),
      .imm_s_sext(imm_s_sext),
      .imm_j_sext(imm_j_sext),
      .imm_u_sext(imm_u_sext),
      /* output */
      .dout      (mux2_dout)
  );

  /* alu */
  riscv_alu alu (
      /* input */
      .exec_fun(exec_fun),
      .data1   (mux1_dout),
      .data2   (mux2_dout),
      /* output */
      .alu_out (alu_dout),
      .br_flag  (br_flag)
  );

  /* csr_regs */
  riscv_csr_regs csr_regs (
      /* input */
      .clk         (clk),
      .csr_write_en(csr_wen),
      .csr_addr    (csr_addr),
      .csr_data    (csr_alu_out),
      .csr_fun     (csr_fun),
      /* output */
      .csr_dout    (csr_dout)
  );

  /* csr_alu */
  riscv_csr_alu csr_alu (
      /* input */
      .reg_data   (mux1_dout),
      .csr_data   (csr_dout),
      .csr_fun    (csr_fun),
      /* output */
      .csr_alu_out(csr_alu_out)
  );

  /* wb mux */
  riscv_wb_mux wb_mux (
      /* input */
      .wb_sel  (wb_sel),
      .alu_out (alu_dout),
      .data    (mask2_out),
      .pc_plus4(pc_plus4),
      .csr_dout(csr_dout),
      /* output */
      .dout    (wb_mux_dout)
  );
endmodule
