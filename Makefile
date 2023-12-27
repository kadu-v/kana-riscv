.PHONY: all clean build test sim

SRCDIR    = ./src
SRCS      = counter_4bit.sv
OBJDIR    = ./obj_dir
OBJS      = 
TBDIR     = ./test_bench
TBBENCHES = $(subst ) 

build: $(OBJDIR)/Vcounter_4bit

$(OBJDIR)/V%: $(SRCDIR)/%.sv $(TBDIR)/tb_%.cpp
	verilator --cc $(word 1,$^) --exe $(word 2,$^)
	make -C $(OBJDIR) -f $(@F).mk

clean: 
	rm -rf ./obj_dir
