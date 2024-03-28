module riscv_ram #(
    parameter WORD_LENGTH = 32,
    parameter ADDR_LENGTH = 5,
    parameter NUM_MEM = 16384
) (
    input  logic                      clk,
    /* port for instruction */
    input  logic    [WORD_LENGTH-1:0] pc,
    output logic    [WORD_LENGTH-1:0] inst,
    /* port for data */
    input  logic    [WORD_LENGTH-1:0] addr,
    input  logic                      write_en,
    input  logic    [WORD_LENGTH-1:0] wdata,
    input  MASK_SEL                   ram_mask_sel,
    output logic    [WORD_LENGTH-1:0] dout
);
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
