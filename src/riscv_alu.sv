`include "riscv_constants.sv"

module riscv_alu #(
    parameter WORD_LENGTH = 32
) (
    input  EXEC_FUN                   exec_fun,
    input  logic    [WORD_LENGTH-1:0] data1,
    input  logic    [WORD_LENGTH-1:0] data2,
    output logic    [WORD_LENGTH-1:0] alu_out
);

  always_comb begin
    case (exec_fun)
      ALU_ADD: begin
        alu_out = data1 + data2;
      end
      default: begin
        alu_out = 0;
      end
    endcase
  end

endmodule