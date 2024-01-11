

SRCDIR             = ./src
SRCS               = riscv_decoder.sv riscv_regs.sv riscv_alu.sv riscv_wb_mux.sv riscv_extend.sv riscv_ram.sv  riscv_top.sv
OBJDIR             = ./obj_dir
OBJS               = 
TBDIR              = ./test_bench
INCLUDE            = -I./src -I/usr/share/verilator/include -I/usr/share/verilator/include/vltstd 
TARGETS            = $(patsubst %.sv, $(OBJDIR)/V%, $(SRCS))
TARGETS_LIBS       = $(patsubst %.sv, $(OBJDIR)/V%__ALL.a, $(SRCS))
TRACE_FLAGS        = --trace --trace-params --trace-structs --trace-underscore --trace-max-array  16384 --gate-stmts 4096
TARGET_DIR         = ./target
RISCVTESTS_SRC_DIR = ./target/share/riscv-tests/isa
RISCVTESTS_BIN_DIR = ./target/bin
RISCVTESTS_SRC     = $(filter-out $(wildcard $(RISCVTESTS_SRC_DIR)/*.dump), $(wildcard $(RISCVTESTS_SRC_DIR)/rv32ui-p*))
RISCVTESTS_BIN     = $(patsubst $(RISCVTESTS_SRC_DIR)/%, $(RISCVTESTS_BIN_DIR)/%.bin, $(RISCVTESTS_SRC))
VERILATOR_OBJS     = $(TARGET_DIR)/verilated.o $(TARGET_DIR)/verilated_vcd_c.o 
TEST_SRCS          = $(TARGET_DIR)/tb_riscv_alu_google.cpp $(TARGET_DIR)/tb_riscv_regs_google.cpp $(TARGET_DIR)/tb_riscv_extend_google.cpp $(TARGET_DIR)/tb_riscv_wb_mux_google.cpp
TEST_BIN           = $(patsubst $(TARGET_DIR)/%_google.cpp, $(TARGET_DIR)/%_google, $(TEST_SRCS))
GOOGLE_TEST_LIBS   = -lgtest_main -lgtest -lpthread 
VERILATOR_CXX_OPTIONS = -MMD \
						-DVM_COVERAGE=0 \
						-DVM_SC=0 \
						-DVM_TRACE=1 \
						-faligned-new \
						-fcf-protection=none \
						-Wno-bool-operation \
						-Wno-sign-compare \
						-Wno-uninitialized \
						-Wno-unused-but-set-variable \
						-Wno-unused-parameter \
						-Wno-unused-variable \
						-Wno-shadow \
						-std=c++20 

# g++  -I.  -MMD -I/usr/share/verilator/include -I/usr/share/verilator/include/vltstd -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TRACE=1 -faligned-new -fcf-protection=none -Wno-bool-operation -Wno-sign-compare -Wno-uninitialized -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable -Wno-shadow     -std=c++20  -Os -c -o verilated_vcd_c.o /usr/share/verilator/include/verilated_vcd_c.cpp

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
	verilator --cc $(word 1,$^) --exe $(word 2,$^) $(INCLUDE) -CFLAGS -std=c++20 $(TRACE_FLAGS)
	make -C $(OBJDIR) -f $(@F).mk

.PHONY: googeltest
googletest: $(TEST_BIN)
	@for executable in $^; do \
		$$executable; \
	done

$(TARGET_DIR)/tb_%_google: $(TBDIR)/tb_%_google.cpp $(TARGET_DIR)/V%__ALL.a $(VERILATOR_OBJS)
	g++ $< $(GOOGLE_TEST_LIBS) \
    -o $@ $(VERILATOR_OBJS) $(word 2,$^) \
    -I/usr/share/verilator/include/ \
    -I$(TARGET_DIR) \
 	$(VERILATOR_CXX_OPTIONS)

$(TARGET_DIR)/V%__ALL.a: $(SRCDIR)/%.sv
	verilator --cc $< $(INCLUDE) -CFLAGS -std=c++20 $(TRACE_FLAGS) -Mdir $(TARGET_DIR)
	make -C $(TARGET_DIR) -f $(patsubst $(TARGET_DIR)/%__ALL.a,%.mk,./$@)

$(TARGET_DIR)/%.o: /usr/share/verilator/include/%.cpp
	g++ $(VERILATOR_CXX_OPTIONS) -Os -c -o $@ $<

.PHONY: riscv-tests
riscv-tests: $(TBDIR)/tb_regression.cpp $(TARGET_DIR)/Vriscv_top__ALL.a $(VERILATOR_OBJS) $(RISCVTESTS_BIN) 
	g++ $< $(GOOGLE_TEST_LIBS) \
    -o $(TARGET_DIR)/tb_regression $(VERILATOR_OBJS) $(word 2,$^) \
    -I/usr/share/verilator/include/ \
    -I$(TARGET_DIR) \
 	$(VERILATOR_CXX_OPTIONS)

$(RISCVTESTS_BIN_DIR)/%.bin: $(RISCVTESTS_SRC_DIR)/%
	riscv64-unknown-elf-objcopy -O binary $< $@

.PHONY: riscv-tests-build
riscv-tests-build:
	cd riscv-tests; ./configure --prefix=$(PWD)/target; make -j8; make install

%.sv: $(OBJDIR)/V%
	./$<

.PHONY: clean
clean: 
	rm -rf ./obj_dir
	rm -rf ./vcds
	rm -rf ./target
