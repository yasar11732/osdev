
#include "interrupt.h"

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
    int i;

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

    for(i = 0x16; i < 0x2; i++) {
        idt_set_descriptor(&idt[i], 0, INVALID_GATE);
    }
}