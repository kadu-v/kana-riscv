
`include "riscv_rom.sv"
`include "riscv_bram.v"

`default_nettype none

module riscv_ram #(
    parameter WORD_LENGTH = 32,
    parameter ADDR_LENGTH = 32,
    parameter NUM_MEM = 16384
) (
    input  logic                      clk,
    /* port for instruction */
    input  logic    [WORD_LENGTH-1:0] pc,
    output logic    [WORD_LENGTH-1:0] inst,
    /* port for data */
    input  logic    [ADDR_LENGTH-1:0] addr,
    input  logic                      write_en,
    input  logic    [WORD_LENGTH-1:0] wdata,
    input  MASK_SEL                   ram_mask_sel,
    output logic    [WORD_LENGTH-1:0] dout
);
  // // Instruction memory
  // riscv_rom rom (
  //     /* input */
  //     .clk  (clk),
  //     .pc   (pc),
  //     /* output */
  //     .inst (inst)
  // );


  // // // Block RAM
  // logic [WORD_LENGTH-1:0] din;
  // assign din = ram_mask_sel == MASK_B ? {dout[31: 8], wdata[ 7:0]}:
  //              ram_mask_sel == MASK_H ? {dout[31:16], wdata[15:0]}:
  //              ram_mask_sel == MASK_X ?               wdata[31:0] :
  //              wdata;
  // logic [WORD_LENGTH-1:0] dummy;

  // riscv_bram #(
  //     .WORD_LENGTH(WORD_LENGTH),
  //     .ADDR_LENGTH(ADDR_LENGTH),
  //     .NUM_MEM(NUM_MEM)
  // ) bram (
  //     /* input */
  //     .clk         (clk),
  //     .write_en    (write_en),
  //     .waddr       (addr),
  //     .raddr       (addr),
  //     .wdata       (wdata),
  //     /* output */
  //     .dout        (dummy)
  // );

  logic [7:0] mem[NUM_MEM];
  assign inst = {mem[pc+3], mem[pc+2], mem[pc+1], mem[pc]};
  assign dout = {mem[addr+3], mem[addr+2], mem[addr+1], mem[addr]};

  always_ff @(posedge clk) begin
    if (write_en) begin
      case (ram_mask_sel)
        MASK_B: begin
          mem[addr] <= wdata[7:0];
        end
        MASK_H: begin
          mem[addr]   <= wdata[7:0];
          mem[addr+1] <= wdata[15:8];
        end
        MASK_X: begin
          mem[addr]   <= wdata[7:0];
          mem[addr+1] <= wdata[15:8];
          mem[addr+2] <= wdata[23:16];
          mem[addr+3] <= wdata[31:24];
        end
        default: begin
          mem[addr]   <= wdata[7:0];
          mem[addr+1] <= wdata[15:8];
          mem[addr+2] <= wdata[23:16];
          mem[addr+3] <= wdata[31:24];
        end
      endcase
    end
  end
endmodule
