symbol-file kernel/kernel.bin
target remote localhost:1234
set max-value-size unlimited
b task_a_proc
c
layout src
