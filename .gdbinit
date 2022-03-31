symbol-file kernel/kernel.bin
target remote localhost:1234
set max-value-size unlimited
b *0xC0102329
c
layout asm
layout regs
