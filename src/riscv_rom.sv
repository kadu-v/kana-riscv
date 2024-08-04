// `ifndef RISCV_ROM
// `define RISCV_ROM

// `default_nettype none

module riscv_rom #(
    parameter WORD_LENGTH = 32,
    parameter NUM_MEM = 10
) (
    input  logic                      clk,
    /* port for instruction */
    input  logic    [WORD_LENGTH-1:0] pc,
    output logic    [WORD_LENGTH-1:0] inst
);
  logic [7:0] mem[NUM_MEM-1:0];
  assign inst = {mem[pc+3], mem[pc+2], mem[pc+1], mem[pc]};

  initial begin
    // $readmemh("assets/test.hex", mem);
    $readmemb("assets/test.txt", mem);
  end 
endmodule
// `endif
