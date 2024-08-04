// `ifndef RISCV_MUX1
// `define RISCV_MUX1

// `include "riscv_constants.sv"

// `default_nettype none

module riscv_mux1 #(
    parameter WORD_LENGTH = 32
) (
    /* input */
    input  OP1_SEL                   op1_sel,
    input  logic   [WORD_LENGTH-1:0] rs1_data,
    input  logic   [WORD_LENGTH-1:0] pc,
    input  logic   [WORD_LENGTH-1:0] imm_z_uext,
    /* out */
    output logic   [WORD_LENGTH-1:0] dout
);

  always_comb begin
    case (op1_sel)
      OP1_RS1: begin
        dout = rs1_data;
      end
      OP1_PC: begin
        dout = pc;
      end
      OP1_IMZ: begin
        dout = imm_z_uext;
      end
      default: begin
        dout = 0;
      end
    endcase
  end
endmodule
// `endif