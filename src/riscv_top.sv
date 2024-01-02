`include "riscv_decoder.sv"
`include "riscv_alu.sv"
`include "riscv_regs.sv"
`include "riscv_constants.sv"
`include "riscv_mux2.sv"

module riscv_top (
    input logic clk,
    input logic x_reset,
    input logic write_en
);

  /* pc */
  logic    [ 31:0] pc_out;

  /* ram */
  logic    [ 31:0] inst;
  logic    [ 31:0] ram_dout;

  /* decoder */
  logic            invalid_o;
  EXEC_FUN         exec_fun;
  OP1_SEL          op1_sel;
  OP2_SEL          op2_sel;
  WB_SEL           wb_sel;
  RF_WEN           rf_wen;
  PC_SEL           pc_sel;

  /* register files */
  logic    [  4:0] rs1_addr = inst[19:15];
  logic    [24:20] rs2_addr = inst[24:20];
  logic    [ 11:7] rd_addr = inst[11:7];

  /* extend */
  logic    [ 31:0] imm_i_sext;

  /* mux2 */
  logic    [ 31:0] mux2_dout;

  /* alu */
  logic    [ 31:0] rs1_data;
  logic    [ 31:0] rs2_data;
  logic    [ 31:0] alu_dout;

  /* wb mux */
  logic    [ 31:0] wb_mux_dout;

  /* pc */
  riscv_pc pc (
      /* input */
      .clk       (clk),
      .x_reset   (x_reset),
      .pc_sel    (pc_sel),
      .imm_i_sext(imm_i_sext),
      /* output */
      .pc_out    (pc_out)
  );


  /* ram */
  riscv_ram ram (
      .clk     (clk),
      /* port for instruction */
      .pc      (pc_out),
      .inst    (inst),
      /* port for data */
      .addr    (alu_dout),
      .write_en(0),
      .wdata   (rs2_data),
      .dout    (ram_dout)
  );

  /* decoder */
  riscv_decoder decoder (
      /* input */
      .inst     (inst),
      /* output */
      .invalid_o(invalid_o),
      .exec_fun (exec_fun),
      .op1_sel  (op1_sel),
      .op2_sel  (op2_sel),
      .wb_sel   (wb_sel),
      .rf_wen   (rf_wen),
      .pc_sel   (pc_sel)
  );

  /* register files*/
  riscv_regs regs (
      /* input */
      .clk       (clk),
      .write_en  (write_en),
      .read_addr1(rs1_addr),
      .read_addr2(rs2_addr),
      .data      (alu_dout),
      .write_addr(rd_addr),
      /* output */
      .read_data1(rs1_data),
      .read_data2(rs2_data)
  );

  /* extend */
  riscv_extend extend (
      /* input */
      .imm(inst),
      /* output */
      .imm_i_sext(imm_i_sext)
  );

  /* mux2 */
  riscv_mux2 mux2 (
      /* input */
      .op2_sel(op2_sel),
      .rs2_data(rs2_data),
      .imm_i_sext(imm_i_sext),
      /* output */
      .dout(mux2_dout)
  );

  /* alu */
  riscv_alu alu (
      /* input */
      .exec_fun(exec_fun),
      .data1   (rs1_data),
      .data2   (mux2_dout),
      /* output */
      .alu_out (alu_dout)
  );

  /* wb mux */
  riscv_wb_mux wb_mux (
      /* input */
      .wb_sel (wb_sel),
      .alu_out(alu_dout),
      .data   (ram_dout),
      /* output */
      .dout   (wb_mux_dout)
  );
endmodule
