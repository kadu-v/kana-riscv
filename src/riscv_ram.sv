
`default_nettype none

module riscv_ram #(
    parameter WORD_LENGTH = 32,
    parameter ADDR_LENGTH = 5,
    parameter NUM_MEM = 128
) (
    input  logic                   clk,
    /* port for instruction */
    input  logic [WORD_LENGTH-1:0] pc,
    output logic [WORD_LENGTH-1:0] inst,
    /* port for data */
    input  logic [WORD_LENGTH-1:0] addr,
    input  MEM_WEN                 write_en,
    input  logic [WORD_LENGTH-1:0] wdata,
    output logic [WORD_LENGTH-1:0] dout
);
  logic [7:0] mem[NUM_MEM];

  assign inst = {mem[pc+3], mem[pc+2], mem[pc+1], mem[pc]};
  assign dout = {mem[addr+3], mem[addr+2], mem[addr+1], mem[addr]};

  always_ff @(posedge clk) begin
    if (write_en == MEM_WRITE) begin
      mem[addr]   <= wdata[7:0];
      mem[addr+1] <= wdata[15:8];
      mem[addr+2] <= wdata[23:16];
      mem[addr+3] <= wdata[31:24];
    end else begin
      mem[0] <= 8'b10010011;
      mem[1] <= 8'b00000001;
      mem[2] <= 8'b00010000;
      mem[3] <= 8'b00000000;
      mem[4] <= 8'b10010011;
      mem[5] <= 8'b00000001;
      mem[6] <= 8'b00010000;
      mem[7] <= 8'b00000000;
      mem[8] <= 8'b10010011;
      mem[9] <= 8'b00000001;
      mem[10] <= 8'b00010000;
      mem[11] <= 8'b00000000;
      mem[12] <= 8'b01101111;
      mem[13] <= 8'b00000010;
      mem[14] <= 8'b10000000;
      mem[15] <= 8'b00000000;
    end
  end

  // initial begin
  //   $readmemb("/home/kaduv/Documents/fpga/kana-riscv/src/test.txt", mem);
  //   // $readmemb("./src/test.txt", mem);
  // end 
endmodule
