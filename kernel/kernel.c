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
#include "sched.h"
#include "i386/io.h"

volatile size_t ticks = 0;

__attribute__((aligned(0x10))) static idt_entry_t idt[IDT_MAX_DESCRIPTORS];

static idtr_t idtr;

typedef struct {
    uint32_t low;
    uint32_t high;
} myuint64_t;

static inline uint32_t read_counter() {
    uint32_t low;
    uint32_t high;
    __asm__ __volatile__("rdtsc" : "=eax"(low), "=edx"(high) : :);
    return (high << 16) | (low >> 16) ;
}

static void diff64(myuint64_t *r, myuint64_t *op1, myuint64_t *op2)
{
    int carry;
    uint32_t d1, d2;

    // destination and source can be
    // the same, so this is needed
    d1 = op1->low;
    d2 = op2->low;
    carry = d1<d2;

    r->low = d1-d2;
    d1 = op1->high;
    d2 = op2->high;

    r->high = d1-d2;

}

static void task_a_proc(void) {

    size_t next_print = ticks+100;
    while(1) {
        __asm__ __volatile__("hlt");
        if(ticks > next_print) {
            printf("task_a: %d\n", read_counter());
            next_print = ticks+100;
        }
        schedule();
    }

}

static void task_b_proc(void) {
    size_t next_print = ticks+150;
    while(1) {
        __asm__ __volatile__("hlt");
        if(ticks > next_print) {
            printf("task_b: %d\n", read_counter());
            next_print = ticks+150;
        }
        schedule();
    }
}

// static void task_c_proc(void) {
//     size_t next_print = ticks+25;
//     while(1) {
//         __asm__ __volatile__("hlt");
//         if(ticks > next_print) {
//             printf("task_c:\n");
//             next_print = ticks+25;
//         }
//         schedule();
//     }
// }

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

    init_page_frame_allocator(mmm, num_mmap_entries);
    init_heap();
    // listPCI();
    initialize_multitasking();
    new_task(task_a_proc);
    new_task(task_b_proc);
    // new_task(task_c_proc);
    while(1) {
        __asm__ __volatile__("hlt");
        schedule();
    }
    return;
}
