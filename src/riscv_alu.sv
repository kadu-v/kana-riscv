`include "riscv_constants.sv"

module riscv_alu #(
    parameter WORD_LENGTH = 32
) (
    /* input */
    input  EXEC_FUN                   exec_fun,
    input  logic    [WORD_LENGTH-1:0] data1,
    input  logic    [WORD_LENGTH-1:0] data2,
    /* output */
    output logic    [WORD_LENGTH-1:0] alu_out,
    output logic                      br_flag
);

  logic signed [WORD_LENGTH-1:0] signed_data1;
  logic signed [WORD_LENGTH-1:0] signed_data2;
  assign signed_data1 = data1;
  assign signed_data2 = data2;

  always_comb begin
    case (exec_fun)
      ALU_ADD: begin
        alu_out = data1 + data2;
      end
      ALU_SUB: begin
        alu_out = data1 - data2;
      end
      ALU_SLT: begin
        alu_out = {{31{1'b0}}, signed_data1 < signed_data2};
      end
      default: begin
        alu_out = 0;
      end
    endcase
  end

  always_comb begin
    if (data1 == data2) begin
      br_flag = 1;
    end else begin
      br_flag = 0;
    end
  end
endmodule
