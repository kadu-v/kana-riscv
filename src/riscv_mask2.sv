`include "riscv_defs.sv"
`include "riscv_constants.sv"

`default_nettype none

module riscv_mask2 #(
    parameter WORD_LENGTH = 32
) (
    /* input */
    input  logic    [WORD_LENGTH-1:0] ram_data,
    input  MASK_SEL                   ram_mask_sel,
    /* output */
    output logic    [WORD_LENGTH-1:0] mask_out
);
  logic [WORD_LENGTH-1:0] mask_b_data;
  logic [WORD_LENGTH-1:0] mask_h_data;
  assign mask_b_data = (ram_data & `RG_SB_MASK);
  assign mask_h_data = (ram_data & `RG_SH_MASK);

  always_comb begin
    case (ram_mask_sel)
      MASK_X: begin
        mask_out = ram_data;
      end
      MASK_B: begin
        mask_out = mask_b_data;
      end
      MASK_H: begin
        mask_out = mask_h_data;
      end
      MASK_B_SEXT: begin
        mask_out = {{24{mask_b_data[7]}}, mask_b_data[7:0]};
      end
      MASK_H_SEXT: begin
        mask_out = {{16{mask_h_data[15]}}, mask_h_data[15:0]};
      end
      default: begin
        mask_out = 0;
      end
    endcase
  end
endmodule
