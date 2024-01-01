`include "riscv_decoder.sv"
`include "riscv_alu.sv"
`include "riscv_regs.sv"
`include "riscv_constants.sv"
`include "riscv_mux2.sv"

module riscv_top (
    input logic        clk,
    input logic [31:0] pc,
    input logic        write_en
);

  /* register files */
  logic    [  4:0] rs1_addr = inst[19:15];
  logic    [24:20] rs2_addr = inst[24:20];
  logic    [ 11:7] rd_addr = inst[11:7];

  /* decoder */
  logic            invalid_o;
  EXEC_FUN         exec_fun;
  OP1_SEL          op1_sel;
  OP2_SEL          op2_sel;
  WB_SEL           wb_sel;


  /* alu */
  logic    [ 31:0] rs1_data;
  logic    [ 31:0] rs2_data;
  logic    [ 31:0] alu_dout;

  /* ram */
  logic    [ 31:0] inst;
  logic    [ 31:0] ram_dout;

  /* wb mux */
  logic    [ 31:0] wb_mux_dout;

  /* extend */
  logic    [ 31:0] imm_i_sext;

  /* mux2 */
  logic    [ 31:0] mux2_dout;



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

  /* decoder */
  riscv_decoder decoder (
      /* input */
      .inst     (inst),
      /* output */
      .invalid_o(invalid_o),
      .exec_fun (exec_fun),
      .op1_sel  (op1_sel),
      .op2_sel  (op2_sel),
      .wb_sel   (wb_sel)
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

  /* ram */
  riscv_ram ram (
      .clk     (clk),
      /* port for instruction */
      .pc      (pc),
      .inst    (inst),
      /* port for data */
      .addr    (alu_dout),
      .write_en(1),
      .wdata   (rs2_data),
      .dout    (ram_dout)
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

endmodule
