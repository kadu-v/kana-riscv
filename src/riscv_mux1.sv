`include "riscv_constants.sv"


module riscv_mux1 #(
    parameter WORD_LENGTH = 32
) (
    /* input */
    input  OP1_SEL                   op1_sel,
    input  logic   [WORD_LENGTH-1:0] rs1_data,
    input  logic   [WORD_LENGTH-1:0] pc,
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
      default: begin
        dout = 0;
      end
    endcase
  end
endmodule
