`include "riscv_defs.sv"
`include "riscv_constants.sv"

module riscv_mask #(
    parameter WORD_LENGTH = 32
) (
    /* input */
    input  logic        [WORD_LENGTH-1:0] rs2_data,
    input  RS2_MASK_SEL                   rs2_mask_sel,
    /* output */
    output logic        [WORD_LENGTH-1:0] mask_out
);
  always_comb begin
    case (rs2_mask_sel)
      MASK_X: begin
        mask_out = rs2_data;
      end
      MASK_B: begin
        mask_out = (rs2_data & `RG_SB_MASK);
      end
      MASK_H: begin
        mask_out = (rs2_data & `RG_SH_MASK);
      end
      default: begin
        mask_out = 0;
      end
    endcase
  end

endmodule
