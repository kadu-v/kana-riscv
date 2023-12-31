`include "riscv_defs.sv"

module decoder (
    input logic [31:0] inst,
    output logic invalid_o
);
  logic invalid_i = (inst & `INST_ADD_MASK) == `INST_ADD;
  assign invalid_o = invalid_i;
endmodule
