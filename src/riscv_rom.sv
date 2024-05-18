`default_nettype none

module riscv_rom #(
    parameter WORD_LENGTH = 32,
    parameter ADDR_LENGTH = 5,
    parameter NUM_MEM = 1024 * 4
) (
    input  logic                      clk,
    /* port for instruction */
    input  logic    [WORD_LENGTH-1:0] pc,
    output logic    [WORD_LENGTH-1:0] inst
);
  logic [7:0] mem[NUM_MEM];

  assign inst = {mem[pc+3], mem[pc+2], mem[pc+1], mem[pc]};
endmodule