
#include <stdio.h>
#include <stddef.h>

#include <kernel/tty.h>
#include "vga.h"
#include "interrupt.h"
#include "../PIC.h"
#include "io.h"

extern size_t ticks;
extern uint8_t vga_color;

void idt_set_descriptor(idt_entry_t *t, void *isr, uint8_t flags)
{
    t->isr_low = (uint32_t)isr & 0xFFFF;
    t->kernel_cs = 0x08;
    t->attributes = flags;
    t->isr_high = (uint32_t)isr >> 16;
    t->reserved = 0;
}

void idt_initialize(idt_entry_t *idt)
{
    int i;
    for(i = 0; i < IDT_MAX_DESCRIPTORS; i++)
    {
        idt_set_descriptor(&idt[i], 0, INVALID_GATE);
    }
}

void idt_init_exception_handlers(idt_entry_t *idt)
{
    idt_set_descriptor(&idt[0x0], exception_divide_error, TRAP_GATE);
    idt_set_descriptor(&idt[0x1], exception_debug_exception, TRAP_GATE);
    idt_set_descriptor(&idt[0x2], exception_nmi_interrupt, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x3], exception_breakpoint, TRAP_GATE);
    idt_set_descriptor(&idt[0x4], exception_overflow, TRAP_GATE);
    idt_set_descriptor(&idt[0x5], exception_bound, TRAP_GATE);
    idt_set_descriptor(&idt[0x6], exception_invalid_opcode, TRAP_GATE);
    idt_set_descriptor(&idt[0x7], exception_device_not_available, TRAP_GATE);
    idt_set_descriptor(&idt[0x8], exception_double_fault, TRAP_GATE);
    idt_set_descriptor(&idt[0x9], exception_coprocessor_segment_overrun, TRAP_GATE);
    idt_set_descriptor(&idt[0xA], exception_invalid_tss, TRAP_GATE);
    idt_set_descriptor(&idt[0xB], exception_segment_not_present, TRAP_GATE);
    idt_set_descriptor(&idt[0xC], exception_stack_segment_fault, TRAP_GATE);
    idt_set_descriptor(&idt[0xD], exception_general_protection, TRAP_GATE);
    idt_set_descriptor(&idt[0xE], exception_page_fault, TRAP_GATE);
    idt_set_descriptor(&idt[0xF], 0, INVALID_GATE);
    idt_set_descriptor(&idt[0x10], exception_math_fault, TRAP_GATE);
    idt_set_descriptor(&idt[0x11], exception_alignment_check, TRAP_GATE);
    idt_set_descriptor(&idt[0x12], exception_machine_check, TRAP_GATE);
    idt_set_descriptor(&idt[0x13], exception_simd_exception, TRAP_GATE);
    idt_set_descriptor(&idt[0x14], exception_virtualization_exception, TRAP_GATE);
    idt_set_descriptor(&idt[0x15], exception_control_protection_exception, TRAP_GATE);

}

void idt_init_interrupt_handlers(idt_entry_t *idt)
{
    idt_set_descriptor(&idt[0x20], isr_wrapper0, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x21], isr_wrapper1, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x22], isr_wrapper2, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x23], isr_wrapper3, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x24], isr_wrapper4, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x25], isr_wrapper5, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x26], isr_wrapper6, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x27], isr_wrapper7, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x28], isr_wrapper8, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x29], isr_wrapper9, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x2A], isr_wrapper10, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x2B], isr_wrapper11, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x2C], isr_wrapper12, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x2D], isr_wrapper13, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x2E], isr_wrapper14, INTERRUPT_GATE);
    idt_set_descriptor(&idt[0x2F], isr_wrapper15, INTERRUPT_GATE);
}

void isr_handler(int irq)
{
    switch(irq) {
        case 0:
            ticks++;
            break;
        case 1:
            int scancode = inb(0x60);
            // if(scancode & 128)
                // printf("%X ", scancode & 0x7F);
            printf("%X ", scancode);

        break;
    }

    PIC_sendEOI(irq);
}

char *exception_messages[] = {
    "Division By Zero Exception",
    "Debug Exception",
    "Non Maskable Interrupt Exception",
    "Breakpoint Exception",
    "Into Detected Overflow Exception",
    "Out of Bounds Exception",
    "Invalid Opcode Exception",
    "No Coprocessor Exception",
    "Double Fault Exception",
    "Coprocessor Segment Overrun Exception",
    "Bad TSS Exception",
    "Segment Not Present Exception",
    "Stack Fault Exception",
    "General Protection Fault Exception",
    "Page Fault Exception",
    "Unknown Interrupt Exception",
    "Coprocessor Fault Exception",
    "Alignment Check Exception (486+)",
    "Machine Check Exception (Pentium/586+)",
    "SIMD Exception",
    "Virtualization Exception",
    "Control Protection Exception",
};

void exception_handler(
    unsigned long exc_no,
    unsigned long edi,
    unsigned long esi,
    unsigned long ebp,
    unsigned long esp,
    unsigned long ebx,
    unsigned long edx,
    unsigned long ecx,
    unsigned long eax,
    unsigned long err_code,
    unsigned long eip,
    unsigned long cs,
    unsigned long eflags
) {
    vga_color = VGA_COLOR(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    terminal_initialize();
    char *exception_message;
    if(exc_no > 21)
        exception_message = "Unknown Exception";
    else
        exception_message = exception_messages[exc_no];

    printf("%s at %X (Segment: %X)\n", exception_message, eip, cs);
    printf("Flags: %X\n", eflags);
    printf("Error Code: %X\n\n", err_code);

    printf("EAX: %X\n", eax);
    printf("EBX: %X\n", ebx);
    printf("ECX: %X\n", ecx);
    printf("EDX: %X\n", edx);

    printf("EBP: %X, ESP: %X, EDI: %X, ESI: %X", ebp, esp, edi, esi);

    for(;;) {
        __asm__ __volatile__ ("cli");
        __asm__ __volatile__ ("hlt");
    }
        
}
