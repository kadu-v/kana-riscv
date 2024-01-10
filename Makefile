

SRCDIR             = ./src
SRCS               = riscv_decoder.sv riscv_regs.sv riscv_alu.sv riscv_wb_mux.sv riscv_extend.sv riscv_ram.sv  riscv_top.sv
OBJDIR             = ./obj_dir
OBJS               = 
TBDIR              = ./test_bench
INCLUDE            = -I./src
TARGETS            = $(patsubst %.sv, $(OBJDIR)/V%, $(SRCS))
TRACE_FLAGS        = --trace --trace-params --trace-structs --trace-underscore --trace-max-array 4096 --gate-stmts 4096
RISCVTESTS         = add addi and andi auipc beq
RISCVTESTS_SRC_DIR = ./target/share/riscv-tests/isa
RISCVTESTS_BIN_DIR = ./target/bin
RISCVTESTS_SRC     = $(filter-out $(wildcard $(RISCVTESTS_SRC_DIR)/*.dump), $(wildcard $(RISCVTESTS_SRC_DIR)/rv32ui-p*))
RISCVTESTS_BIN     = $(patsubst $(RISCVTESTS_SRC_DIR)/%, $(RISCVTESTS_BIN_DIR)/%.bin, $(RISCVTESTS_SRC))

$(shell mkdir -p $(RISCVTESTS_BIN_DIR))

.PHONY: build
build: $(TARGETS)

.PHONY: test
test: $(TARGETS)
	@for executable in $^; do \
		echo "/*--------------------Running $$executable"--------------------*/; \
		$$executable; \
	done

$(OBJDIR)/V%: $(SRCDIR)/%.sv $(TBDIR)/tb_%.cpp
	mkdir -p ./vcds
	verilator --cc $(word 1,$^) --exe $(word 2,$^) $(INCLUDE) -CFLAGS -std=c++20 $(TRACE_FLAGS)
	make -C $(OBJDIR) -f $(@F).mk

.PHONY: riscv-tests
riscv-tests: $(RISCVTESTS_BIN)
	cd riscv-tests; ./configure --prefix=$(PWD)/target; make -j8; make install
	mkdir -p ./vcds
	verilator --cc $(SRCDIR)/riscv_top.sv --exe $(TBDIR)/tb_regression.cpp $(INCLUDE) -CFLAGS -std=c++20 $(TRACE_FLAGS) -o Vtb_regression
	make -C $(OBJDIR) -f Vriscv_top.mk
	@for bin_file in $^; do \
		echo "/*--------------------Running $$bin_file"--------------------*/; \
		$(OBJDIR)/Vtb_regression $$bin_file; \
	done

$(RISCVTESTS_BIN_DIR)/%.bin: $(RISCVTESTS_SRC_DIR)/%
	riscv64-unknown-elf-objcopy -O binary $< $@

%.sv: $(OBJDIR)/V%
	./$<

.PHONY: clean
clean: 
	rm -rf ./obj_dir
	rm -rf ./vcds
	rm -rf ./target
