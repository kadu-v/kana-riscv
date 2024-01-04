`include "riscv_constants.sv"


module riscv_mux1 (
    /* input */
    input  OP1_SEL        op1_sel,
    input  logic   [31:0] rs1_data,
    input  logic   [31:0] pc,
    /* output */
    output logic   [31:0] dout
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
