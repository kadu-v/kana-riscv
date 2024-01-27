`include "riscv_constants.sv"

`default_nettype none

module riscv_mux2 #(
    parameter WORD_LENGTH = 32
) (
    /* input */
    input  OP2_SEL                   op2_sel,
    input  logic   [WORD_LENGTH-1:0] rs2_data,
    input  logic   [WORD_LENGTH-1:0] imm_i_sext,
    input  logic   [WORD_LENGTH-1:0] imm_s_sext,
    input  logic   [WORD_LENGTH-1:0] imm_j_sext,
    input  logic   [WORD_LENGTH-1:0] imm_u_sext,
    /* output */
    output logic   [WORD_LENGTH-1:0] dout
);

  always_comb begin
    case (op2_sel)
      OP2_RS2: begin
        dout = rs2_data;
      end
      OP2_IMI: begin
        dout = imm_i_sext;
      end
      OP2_IMS: begin
        dout = imm_s_sext;
      end
      OP2_IMJ: begin
        dout = imm_j_sext;
      end
      OP2_IMU: begin
        dout = imm_u_sext;
      end
      default: begin
        dout = 0;
      end
    endcase
  end
endmodule
