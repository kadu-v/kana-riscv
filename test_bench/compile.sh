#/usr/bin/bash

g++ tb_google_test.cpp \
    -lgtest_main \
    -lgtest \
    -lpthread \
    -o test ../obj_dir/verilated.o ../obj_dir/verilated_vcd_c.o ../obj_dir/Vriscv_alu__ALL.a \
    -I/usr/share/verilator/include/ \
    -I../obj_dir -std=c++20 