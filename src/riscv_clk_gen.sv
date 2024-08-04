// `ifndef RISCV_CLK_GEN
// `define RISCV_CLK_GEN


// `default_nettype none
module riscv_clk_gen #(
    parameter DIV = 3
) (
    /* input */
    input  logic clk_i,
    input  logic x_reset,
    /* output */
    output logic clk_o
);
    logic [DIV-1:0] cnt;
    always_ff @(posedge clk_i) begin
        if (!x_reset) begin
            cnt   <= 0;
            clk_o <= 1;
        end else begin
            if (cnt == DIV-1) begin
                cnt   <= 0;
                clk_o <= ~clk_o;
            end else begin
                cnt   <= cnt + 1;
            end
        end
    end
endmodule
// `endif