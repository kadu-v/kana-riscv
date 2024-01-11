# kana-riscv

**K**indly **Ana**log Pocket **RISC-V** Core


mkdir -p ./vcds
verilator --cc src/riscv_decoder.sv --exe test_bench/tb_riscv_decoder.cpp -I./src -CFLAGS -std=c++20 --trace --trace-params --trace-structs --trace-underscore --trace-max-array  16384 --gate-stmts 4096
make -C ./obj_dir -f Vriscv_decoder.mk
make[1]: Entering directory '/workspaces/kana-riscv/obj_dir'
g++  -I.  -MMD -I/usr/share/verilator/include -I/usr/share/verilator/include/vltstd -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TRACE=1 -faligned-new -fcf-protection=none -Wno-bool-operation -Wno-sign-compare -Wno-uninitialized -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable -Wno-shadow     -std=c++20  -Os -c -o tb_riscv_decoder.o ../test_bench/tb_riscv_decoder.cpp
g++  -I.  -MMD -I/usr/share/verilator/include -I/usr/share/verilator/include/vltstd -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TRACE=1 -faligned-new -fcf-protection=none -Wno-bool-operation -Wno-sign-compare -Wno-uninitialized -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable -Wno-shadow     -std=c++20  -Os -c -o verilated.o /usr/share/verilator/include/verilated.cpp
g++  -I.  -MMD -I/usr/share/verilator/include -I/usr/share/verilator/include/vltstd -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TRACE=1 -faligned-new -fcf-protection=none -Wno-bool-operation -Wno-sign-compare -Wno-uninitialized -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable -Wno-shadow     -std=c++20  -Os -c -o verilated_vcd_c.o /usr/share/verilator/include/verilated_vcd_c.cpp
/usr/bin/perl /usr/share/verilator/bin/verilator_includer -DVL_INCLUDE_OPT=include Vriscv_decoder.cpp Vriscv_decoder___024unit.cpp Vriscv_decoder__Trace.cpp Vriscv_decoder__Slow.cpp Vriscv_decoder___024unit__Slow.cpp Vriscv_decoder__Syms.cpp Vriscv_decoder__Trace__Slow.cpp > Vriscv_decoder__ALL.cpp
g++  -I.  -MMD -I/usr/share/verilator/include -I/usr/share/verilator/include/vltstd -DVM_COVERAGE=0 -DVM_SC=0 -DVM_TRACE=1 -faligned-new -fcf-protection=none -Wno-bool-operation -Wno-sign-compare -Wno-uninitialized -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable -Wno-shadow     -std=c++20  -Os -c -o Vriscv_decoder__ALL.o Vriscv_decoder__ALL.cpp
ar -cr Vriscv_decoder__ALL.a Vriscv_decoder__ALL.o
ranlib Vriscv_decoder__ALL.a
g++    tb_riscv_decoder.o verilated.o verilated_vcd_c.o Vriscv_decoder__ALL.a      -o Vriscv_decoder