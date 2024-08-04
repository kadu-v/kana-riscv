// `ifndef RISCV_MASK1
// `define RISCV_MASK1

// `include "riscv_defs.sv"
// `include "riscv_constants.sv"

// `default_nettype none

module riscv_mask1 #(
    parameter WORD_LENGTH = 32
) (
    /* input */
    input  logic    [WORD_LENGTH-1:0] rs2_data,
    input  MASK_SEL                   rs2_mask_sel,
    /* output */
    output logic    [WORD_LENGTH-1:0] mask_out
);
  logic [WORD_LENGTH-1:0] mask_b_data;
  logic [WORD_LENGTH-1:0] mask_h_data;
  assign mask_b_data = (rs2_data & `RG_SB_MASK);
  assign mask_h_data = (rs2_data & `RG_SH_MASK);

  always_comb begin
    case (rs2_mask_sel)
      MASK_X: begin
        mask_out = rs2_data;
      end
      MASK_B: begin
        mask_out = mask_b_data;
      end
      MASK_H: begin
        mask_out = mask_h_data;
      end
      default: begin
        mask_out = 0;
      end
    endcase
  end
endmodule
// `endif