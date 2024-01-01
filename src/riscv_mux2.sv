`include "riscv_constants.sv"


module riscv_mux2 (
    input  OP2_SEL        op2_sel,
    input  logic   [31:0] rs2_data,
    input  logic   [31:0] imm_i_sext,
    output logic   [31:0] dout
);

  always_comb begin
    case (op2_sel)
      OP2_RS2: begin
        dout = rs2_data;
      end
      OP2_IMS: begin
        dout = imm_i_sext;
      end
      default: begin
        dout = 0;
      end
    endcase
  end

endmodule
