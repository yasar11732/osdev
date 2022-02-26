
/* Constants */
.set ALIGN, 1 /* Page Alignment */
.set MEMINFO, 1<<1 /* Provide Meminfo */
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
_stack_bottom:
.skip 16384
_stack_top:

.section .text
.global _start
.type _start, @function
_start:

    // LOAD GDT
    lgdt (gdtr)

    // refresh segment registers
    jmp $0x08,$_reload_segment_registers // for CS
_reload_segment_registers:
    mov $0x10, %cx
    mov %cx,%ds
    mov %cx,%es
    mov %cx,%fs
    mov %cx,%gs
    mov %cx,%ss

    mov $_stack_top, %esp
    // Save ebx and eax so _init doesnt trash it
    push %ebx
    push %eax
    
    cld

    call _init /* Global Constructors */

    // TODO: Install IDT Here

    call kernel_main

sleep:
    hlt
    jmp sleep

.align 4
.global outb
.type outb, @function
outb:
	movl	8(%esp), %eax
	movl	4(%esp), %edx
	out    %al,(%dx)
	ret

.align 4
.global outh
.type outh, @function	
outh:
	movl	8(%esp), %eax
	movl	4(%esp), %edx
	out   %ax,(%dx)
	ret

.align 4
.global out
.type out, @function	
out:
	movl	8(%esp), %eax
	movl	4(%esp), %edx
	out    %eax,(%dx)
	ret

.align 4
.global inb
.type inb, @function
inb:
    movl 4(%esp),%edx
    xor %eax,%eax
    in (%dx),%al
    ret

.align 4
.global inh
.type inh, @function
inh:
    movl 4(%esp),%edx
    xor %eax,%eax
    in (%dx),%ax
    ret

.align 4
.global in
.type in, @function
in:
    movl 4(%esp),%edx
    xor %eax,%eax
    in (%dx),%eax
    ret

.size _start, . - _start

.section .rodata
.align 4
gdt:
    .long 0x0 // null descriptor
    .long 0x0
    .long 0xFFFF // kernel mode code segment
    .long 0xCF9A00
    .long 0xFFFF // kernel mode data segment
    .long 0xCF9200
gdtr:
    .word . - gdt - 1 // limit
    .long gdt // base