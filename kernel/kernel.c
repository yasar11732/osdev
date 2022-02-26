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
#include <kernel/tty.h>

#include "multiboot.h"
#include "i386/interrupt.h"
#include "i386/boot.h"

char *memtypes[] = {"","Available","Reserved","Acpi Reclaimable","NVS","Bad Ram"};

__attribute__((aligned(0x10))) static idt_entry_t idt[IDT_MAX_DESCRIPTORS];

static idtr_t idtr;

void PIC_initialize(int master_offset, int slave_offset)
{
    
}

void kernel_main(uint32_t magic, multiboot_info_t *multiboot_info) {

    //disable PIC's until we are ready to initialize
    outb(0xa1, 0xff);
    outb(0x21, 0xff);

    unsigned int i;

    terminal_initialize();

    idt_initialize(idt);
    idt_init_exception_handlers(idt);

    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint32_t)(&idt[0]);
    
    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag

    if(magic != 0x2BADB002) {
        printf("Must be loaded from multiboot loader.\r\n");
        return;
    }

    if(! (multiboot_info->flags >> 6 & 1) ) {
        printf("Multiboot didn't provide a memory map.\r\n");
        return;
    }

    printf("MEMORY MAP\n");

    multiboot_memory_map_t *mmm = (multiboot_memory_map_t *)multiboot_info->mmap_addr;
    for(i = 0; i < (multiboot_info->mmap_length / sizeof(multiboot_memory_map_t));i++)
    {
        uint32_t start = mmm[i].addr_low;
        uint32_t end = start + mmm[i].len_low - 1;

        printf("        %X - %X: %s\n",
        start, end,
        memtypes[mmm[i].type]);

        if(mmm[i].type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            size_t gigs = mmm[i].len_low;
            size_t bytes = gigs & 0x3FF;
            gigs >>= 10;
            size_t kbytes = gigs & 0x3FF;
            gigs >>= 10;
            size_t mbytes = gigs & 0x3FF;
            gigs >>=10;

            printf("%d, %d GB - %d MB - %d KB - %d B\n", mmm[i].len_low, gigs, mbytes, kbytes, bytes);
        }
    }
    return;
}