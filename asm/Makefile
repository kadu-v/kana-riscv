.PHONY: build clean


build:
	riscv64-unknown-elf-gcc -o test.elf test.S -march=rv32i -mabi=ilp32 -nostdlib -nostartfiles -T ./link.ld
	od -An -tx1 -w1 -v ./test.elf > test.hex

clean:
	rm -rf ./*.elf
	rm -rf ./*.hex