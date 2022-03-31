
/* Constants */
.set ALIGN, 1 /* Page Alignment */
.set MEMINFO, 1<<1 /* Provide Meminfo */
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot, "a"
.long MAGIC
.long FLAGS
.long CHECKSUM

// allocate 16KB for stack 4 KB each for page directory and 1 table
.section .bss
    .align 16
_stack_bottom:
    .skip 16384
_stack_top:
    .align 4096

.section .pagestructs, "aw",@nobits
kernel_page_directory:
    .skip 4096
kernel_page_table:
    .skip 4096

.section .earlyinit, "ax"
.global _early_init
.type _start, @function
_early_init:
    
    # we will use physical adress of stack
    # pointer until paging is enabled
    mov $(_stack_top - 0xC0000000), %esp

    # ebx holds the physical adress of
    # multiboot info, convert it to virtual
    # address and push it
    add $0xC0000000,%ebx
    push %ebx

    # push magic number provided by multiboot
    push %eax

    # create initial page mapping
    # eax: loop counter
    # ecx: pointer to page table (physical)
    # edx: page table entry
    
    xor %eax,%eax
    mov $(kernel_page_table - 0xC0000000), %ecx
    // map low memory read-write
L1:
    mov %eax,%edx
    shl $0xc,%edx
    or $0x3,%edx
    mov %edx, (%ecx,%eax,4)
    add $1,%eax
    cmp $0x100,%eax
    jne L1

    // map .earlyinit .text and .rodata read-only
    mov $_data_section_start,%ebx
    sub $0xC0000000,%ebx
    shr $0xC,%ebx
L2:
    mov %eax,%edx
    shl $0xc,%edx
    add $1,%edx
    mov %edx, (%ecx,%eax,4)
    add $1,%eax
    cmp %ebx, %eax
    jne L2

    // map until end of kernel read-write
    mov $kernel_page_directory,%ebx
    sub $0xC0000000,%ebx
    shr $0xC,%ebx
L3:
    mov %eax,%edx
    shl $0xc,%edx
    or $0x3,%edx
    mov %edx, (%ecx,%eax,4)
    add $1,%eax
    cmp %ebx, %eax
    jne L3
    cmp $0x400, %eax
    jge L5
    // zero fill rest, just in case
L4:
    movl $0, (%ecx,%eax,4)
    add $1,%eax
    cmp $0x400, %eax
    jne L4
L5:
    #use same table for 0x00000000 and 0xC0000000
    # 0xC0000000 corresponds to 768th entry
    mov $(kernel_page_directory - 0xc0000000), %edx
    or $0x3,%ecx
    mov %ecx, (%edx)
    mov %ecx, (768 * 4)(%edx)

    # recursive mapping
    mov %edx,%ecx
    or $0x3,%ecx
    mov %ecx, (1023 *4 )(%edx)

    # set page directory adress
    mov %edx,%cr3

    # enable paging
    mov %cr0,%eax
    or $0x80010000,%eax
    mov %eax,%cr0

    // long jump
    mov $_paged_start,%eax
    jmp *%eax

.section .text
_paged_start:
    
    // fix the stack pointer
    add $0xC0000000, %esp

    // unmap lower half
    // page table is mapped to 0xFFFFF000 now
    movl $0xFFFFF000,%edx
    movl $0,(%edx)

    // refresh tlb
    mov %cr3,%edx
    mov %edx,%cr3

    // LOAD GDT
    lgdt (gdtr)

    // refresh segment registers
    jmp $0x08,$_reload_segment_registers // for CS
_reload_segment_registers:
    mov $0x10, %ecx
    mov %ecx,%ds
    mov %ecx,%es
    mov %ecx,%fs
    mov %ecx,%gs
    mov %ecx,%ss

    cld
    call kernel_main

sleep:
    hlt
    jmp sleep


.section .data
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