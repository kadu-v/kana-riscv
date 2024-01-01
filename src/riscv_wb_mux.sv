`include "riscv_constants.sv"

module riscv_wb_mux #(
    parameter WORD_LENGTH = 32
) (
    input  WB_SEL                   wb_sel,
    input  logic  [WORD_LENGTH-1:0] alu_out,
    input  logic  [WORD_LENGTH-1:0] data,
    output logic  [WORD_LENGTH-1:0] dout
);

  always_comb begin
    case (wb_sel)
      WB_MEM: begin
        dout = data;
      end
      default: begin
        dout = alu_out;
      end
    endcase
  end

endmodule
