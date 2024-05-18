SRC_DIR               = ./src
SRCS                  = riscv_decoder.sv riscv_regs.sv riscv_alu.sv riscv_wb_mux.sv riscv_extend.sv riscv_rom.sv riscv_ram.sv riscv_top.sv 
TB_DIR                = ./test_bench
INCLUDE               = -I./src -I/usr/share/verilator/include -I/usr/share/verilator/include/vltstd 
TARGET_DIR            = ./target
VCD_DIR               = ./vcds

# unit tests
TEST_SRCS             = $(wildcard $(TB_DIR)/tb_*.cpp)
TEST_BIN              = $(patsubst $(TB_DIR)/%.cpp, $(TARGET_DIR)/%, $(TEST_SRCS))
TEST_LIBS             = $(patsubst %.sv, $(OBJDIR)/V%__ALL.a, $(SRCS))

# riscv-tests
RISCVTESTS_SRC_DIR    = ./target/share/riscv-tests/isa
RISCVTESTS_SRC        = $(filter-out $(wildcard $(RISCVTESTS_SRC_DIR)/*.dump), $(wildcard $(RISCVTESTS_SRC_DIR)/rv32ui-p*))
RISCVTESTS_BIN_DIR    = ./target/bin
RISCVTESTS_BIN        = $(patsubst $(RISCVTESTS_SRC_DIR)/%, $(RISCVTESTS_BIN_DIR)/%.bin, $(RISCVTESTS_SRC))

# general settings for tests
GOOGLE_TEST_LIBS      = -lgtest_main -lgtest -lpthread 
VERILATOR_FLAGS       = --trace --trace-params --trace-structs --trace-underscore --trace-max-array  16384 --gate-stmts 4096
VERILATOR_OBJS        = $(TARGET_DIR)/verilated.o $(TARGET_DIR)/verilated_vcd_c.o 
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

$(shell mkdir -p $(TARGET_DIR))
$(shell mkdir -p $(RISCVTESTS_BIN_DIR))
$(shell mkdir -p $(VCD_DIR))

.PHONY: build 
build: riscv-tests-build

.PHONY: test
test: $(TEST_BIN)
	@for executable in $^; do \
		$$executable; \
	done

$(TARGET_DIR)/tb_%: $(TB_DIR)/tb_%.cpp $(TARGET_DIR)/V%__ALL.a $(VERILATOR_OBJS)
	g++ $< $(GOOGLE_TEST_LIBS) \
    -o $@ $(VERILATOR_OBJS) $(word 2,$^) \
    -I/usr/share/verilator/include/ \
    -I$(TARGET_DIR) \
 	$(VERILATOR_CXX_OPTIONS)

$(TARGET_DIR)/V%__ALL.a: $(SRC_DIR)/%.sv
	verilator --cc $< $(INCLUDE) -CFLAGS -std=c++20 $(VERILATOR_FLAGS) -Mdir $(TARGET_DIR)
	make -C $(TARGET_DIR) -f $(patsubst $(TARGET_DIR)/%__ALL.a,%.mk,./$@)

$(TARGET_DIR)/%.o: /usr/share/verilator/include/%.cpp
	g++ $(VERILATOR_CXX_OPTIONS) -Os -c -o $@ $<

.PHONY: riscv-tests
riscv-tests: $(TB_DIR)/riscv_tests.cpp $(TARGET_DIR)/Vriscv_top__ALL.a $(VERILATOR_OBJS) $(RISCVTESTS_BIN) 
	g++ $< $(GOOGLE_TEST_LIBS) \
    -o $(TARGET_DIR)/tb_regression $(VERILATOR_OBJS) $(word 2,$^) \
    -I/usr/share/verilator/include/ \
    -I$(TARGET_DIR) \
 	$(VERILATOR_CXX_OPTIONS)
	$(TARGET_DIR)/tb_regression

$(RISCVTESTS_BIN_DIR)/%.bin: $(RISCVTESTS_SRC_DIR)/%
	riscv64-unknown-elf-objcopy -O binary $< $@

.PHONY: riscv-tests-build
riscv-tests-build:
	cd riscv-tests; ./configure --prefix=$(PWD)/target; make -j8; make install

%.sv: $(OBJDIR)/V%
	./$<

.PHONY: clean
clean: 
	rm -rf ./vcds
	rm -rf ./target
