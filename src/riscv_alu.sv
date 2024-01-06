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
      ALU_SLL: begin
        alu_out = (data1 << data2[4:0]);
      end
      ALU_SLT: begin
        alu_out = {{31{1'b0}}, signed_data1 < signed_data2};
      end
      ALU_SLTU: begin
        alu_out = {{31{1'b0}}, data1 < data2};
      end
      ALU_XOR: begin
        alu_out = data1 ^ data2;
      end
      ALU_SRL: begin
        alu_out = (data1 >> data2[4:0]);
      end
      ALU_SRA: begin
        alu_out = (signed_data1 >> data2[4:0]);
      end
      ALU_OR: begin
        alu_out = data1 | data2;
      end
      ALU_AND: begin
        alu_out = data1 & data2;
      end
      ALU_JALR: begin
        alu_out = (data1 + data2) & ~32'b1;
      end
      default: begin
        alu_out = 0;
      end
    endcase
  end

  always_comb begin
    case (exec_fun)
      ALU_BEQ: begin
        br_flag = data1 == data2;
      end
      ALU_BNE: begin
        br_flag = data1 != data2;
      end
      ALU_BLT: begin
        br_flag = signed_data1 < signed_data2;
      end
      ALU_BGE: begin
        br_flag = signed_data1 >= signed_data2;
      end
      ALU_BLTU: begin
        br_flag = data1 < data2;
      end
      ALU_BGEU: begin
        br_flag = data1 >= data2;
      end
      default: begin
        br_flag = 0;
      end
    endcase
  end
endmodule
