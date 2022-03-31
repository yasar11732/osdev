#if defined (__linux__)
#error "USE CROSS COMPILER!!"
#endif

#if !defined(__i386__)
#error "Need to use ix86-elf compiler"
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <kernel/tty.h>
#include "multiboot.h"
#include "i386/interrupt.h"
#include "i386/io.h"
#include "com.h"
#include "PIC.h"
#include "page_frame_allocator.h"
#include "heap.h"
#include "i386/pci.h"

volatile size_t ticks = 0;




char *memtypes[] = {"","Available","Reserved","Acpi Reclaimable","NVS","Bad Ram"};

__attribute__((aligned(0x10))) static idt_entry_t idt[IDT_MAX_DESCRIPTORS];

static idtr_t idtr;

void kernel_main(uint32_t magic, multiboot_info_t *multiboot_info) {
    unsigned long i;

    if(magic != 0x2BADB002) {
        printf("Must be loaded from multiboot loader.\r\n");
        return;
    }

    if(! (multiboot_info->flags >> 6 & 1) ) {
        printf("Multiboot didn't provide a memory map.\r\n");
        return;
    }

    // init PIT
    outb(0x43, 0b00110100); // BCD - Rate Generator - lobyte/hibyte - Channel 0

    // 0x38D7 -> 82HZ
    outb(0x40, 0xD7);
    outb(0x40, 0x38);

    // remap PIC1 to 0x20 PIC2 to 0x28
    PIC_remap(0x20,0x28);

    terminal_initialize();

    idt_initialize(idt);
    idt_init_exception_handlers(idt);
    idt_init_interrupt_handlers(idt);

    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint32_t)(&idt[0]);
    
    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag

    init_serial(COM1, 3);

    printf("MEMORY MAP\n");


    multiboot_memory_map_t *mmm = multiboot_info->mmap_addr + 0xC0000000;
    unsigned long num_mmap_entries = multiboot_info->mmap_length / sizeof(multiboot_memory_map_t);

    /*
    for(i = 0; i < num_mmap_entries; i++)
    {
            unsigned long start = mmm[i].addr_low;
            unsigned long len   = mmm[i].len_low;

            printf("%X-%X: Type %d\n", start, start+len-1, mmm[i].type);
    }*/

    init_page_frame_allocator(mmm, num_mmap_entries);
    init_heap();
    listPCI();
    return;
}
