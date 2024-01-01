.PHONY: all clean build test sim

SRCDIR     = ./src
SRCS       = counter_4bit.sv riscv_decoder.sv riscv_regs.sv
OBJDIR     = ./obj_dir
OBJS       = 
TBDIR      = ./test_bench
INCLUDE    = -I./src
TARGETS    = $(patsubst %.sv, $(OBJDIR)/V%, $(SRCS))

build: $(TARGETS)

$(OBJDIR)/V%: $(SRCDIR)/%.sv $(TBDIR)/tb_%.cpp
	verilator --cc $(word 1,$^) --exe $(word 2,$^) $(INCLUDE) -CFLAGS -std=c++2a
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
