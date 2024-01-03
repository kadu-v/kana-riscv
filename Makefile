.PHONY: all clean build test sim

SRCDIR      = ./src
SRCS        = riscv_decoder.sv riscv_regs.sv riscv_alu.sv riscv_wb_mux.sv riscv_extend.sv riscv_ram.sv  riscv_top.sv
OBJDIR      = ./obj_dir
OBJS        = 
TBDIR       = ./test_bench
INCLUDE     = -I./src
TARGETS     = $(patsubst %.sv, $(OBJDIR)/V%, $(SRCS))
TRACE_FLAGS = --trace --trace-params --trace-structs --trace-underscore

build: $(TARGETS)

$(OBJDIR)/V%: $(SRCDIR)/%.sv $(TBDIR)/tb_%.cpp
	mkdir -p ./vcds
	verilator --cc $(word 1,$^) --exe $(word 2,$^) $(INCLUDE) -CFLAGS -std=c++2a $(TRACE_FLAGS)
	make -C $(OBJDIR) -f $(@F).mk

test: $(TARGETS)
	@for executable in $^; do \
		echo "/*--------------------Running $$executable"--------------------*/; \
		$$executable; \
	done

%.sv: $(OBJDIR)/V%
	./$<
	
clean: 
	rm -rf ./obj_dir
	rm -rf ./vcds
