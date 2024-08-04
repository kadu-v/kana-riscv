// `ifndef RISCV_CSR_ALU
// `define RISCV_CSR_ALU

// `include "riscv_constants.sv"

// `default_nettype none

module riscv_csr_alu #(
    parameter WORD_LENGTH = 32
) (
    /* input */
    input  logic   [WORD_LENGTH-1:0] reg_data,
    input  logic   [WORD_LENGTH-1:0] csr_data,
    input  CSR_FUN                   csr_fun,
    /* output */
    output logic   [WORD_LENGTH-1:0] csr_alu_out
);

  always_comb begin
    case (csr_fun)
      CSR_W: begin
        csr_alu_out = reg_data;
      end
      CSR_S: begin
        csr_alu_out = reg_data | csr_data;
      end
      CSR_C: begin
        csr_alu_out = csr_data & ~reg_data;
      end
      default: begin
        csr_alu_out = 0;
      end
    endcase
  end
endmodule
// `endif