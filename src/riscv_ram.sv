// `ifndef RISCV_RAM
// `define RISCV_RAM

// `include "riscv_rom.sv"
// `include "riscv_bram.v"

// `default_nettype none

typedef enum {
  SUSPEND,
  INIT,
  S0,
  S1,
  S2,
  S3,
  LAST
} State;

module riscv_ram #(
    parameter WORD_LENGTH = 32,
    parameter ADDR_LENGTH = 32,
    parameter NUM_MEM = 1024 * 1024 * 1024 * 1024 * 1024 * 1024
) (
    input  logic                      clk,
    input  logic                      clk3,
    input logic                       x_reset,
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
  // Instruction memory
  riscv_rom rom (
      /* input */
      .clk  (clk3),
      .pc   (pc),
      /* output */
      .inst (inst)
  );

  // Block RAM
  assign dout = tmp_dout;
  logic [WORD_LENGTH-1:0] din;

  State state;
  logic                   bram_write_en;
  logic [ADDR_LENGTH-1:0] bram_raddr;
  logic [ADDR_LENGTH-1:0] bram_waddr;
  logic [            7:0] bram_wdata;
  logic [            7:0] bram_dout;
  logic [WORD_LENGTH-1:0] tmp_bram_dout;
  logic [WORD_LENGTH-1:0] tmp_dout;
  always_ff @(posedge clk) begin
    if (!x_reset) begin
      state <= INIT;
    end else begin
      case (state)
          INIT: begin
              bram_write_en <= write_en;
              bram_raddr    <= addr;
              bram_waddr    <= addr;
              bram_wdata    <= wdata[7: 0];
              state         <= S0;
            end
          S0: begin
            bram_write_en       <= ram_mask_sel == MASK_B ? 
                                    0 : write_en;
            bram_raddr          <= bram_raddr + 1;
            bram_waddr          <= bram_waddr + 1;
            bram_wdata          <= wdata[15: 8];
            tmp_bram_dout[7: 0] <= bram_dout;
            state               <= S1;
          end
          S1: begin
            bram_write_en       <= ram_mask_sel == MASK_B || ram_mask_sel == MASK_H ? 
                                    0 : write_en;
            bram_raddr          <= bram_raddr + 1;
            bram_waddr          <= bram_waddr + 1;
            bram_wdata          <= wdata[23:16];
            tmp_bram_dout[15:8] <= bram_dout;
            state               <= S2;
          end
          S2: begin
            bram_raddr           <= bram_raddr + 1;
            bram_waddr           <= bram_waddr + 1;
            bram_wdata           <= wdata[31:24];
            tmp_bram_dout[23:16] <= bram_dout;
            state                <= S3;
          end
          S3: begin
            tmp_bram_dout[31:24] <= bram_dout;
            state                <= LAST;
          end
          LAST: begin
            tmp_dout <= tmp_bram_dout;
            state    <= INIT;
          end
        endcase
    end
  end

  logic debug;
  riscv_bram bram (
      /* input */
      .clk         (clk),
      .write_en    (bram_write_en),
      .waddr       (bram_waddr),
      .raddr       (bram_raddr),
      .wdata       (bram_wdata),
      /* output */
      .debug       (debug),
      .dout        (bram_dout)
  );
endmodule
// `endif