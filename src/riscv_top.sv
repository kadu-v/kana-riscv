`include "riscv_decoder.sv"
`include "riscv_alu.sv"
`include "riscv_regs.sv"
`include "riscv_constants.sv"

module riscv_top (
    input logic clk,
    input logic [31:0] inst,
    input logic write_en,
    output logic [31:0] debug_out
);

  /* register files */
  logic [4:0] rs1_addr = inst[19:15];
  logic [24:20] rs2_addr = inst[24:20];
  logic [11:7] rd_addr = inst[11:7];

  /* decoder */
  logic invalid_o;
  EXEC_FUN exec_fun;
  OP1_SEL op1_sel;
  OP2_SEL op2_sel;

  /* alu */
  logic [31:0] data1;
  logic [31:0] data2;
  logic [31:0] alu_out;


  /* register files*/
  riscv_regs regs (
      .clk(clk),
      .write_en(write_en),
      .read_addr1(rs1_addr),
      .read_addr2(rs2_addr),
      .data(alu_out),
      .write_addr(rd_addr),
      .read_data1(data1),
      .read_data2(data2),
      .debug_out(debug_out)
  );

  /* decoder */
  riscv_decoder decoder (
      .inst(inst),
      .invalid_o(invalid_o),
      .exec_fun(exec_fun),
      .op1_sel(op1_sel),
      .op2_sel(op2_sel)
  );

  /* alu */
  riscv_alu alu (
      .exec_fun(exec_fun),
      .data1(data1),
      .data2(data2),
      .alu_out(alu_out)
  );


endmodule
