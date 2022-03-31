
#ifndef _ARCH_I386_INTERRUPT_H
#define  _ARCH_I386_INTERRUPT_H

#include <stdint.h>

typedef struct {
    uint16_t isr_low;
    uint16_t kernel_cs;
    uint8_t reserved;
    uint8_t attributes;
    uint16_t isr_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr_t;

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
) __attribute__ ((noreturn));

void exception_divide_error();
void exception_debug_exception();
void exception_nmi_interrupt();
void exception_breakpoint();
void exception_overflow();
void exception_bound();
void exception_invalid_opcode();
void exception_device_not_available();
void exception_double_fault();
void exception_coprocessor_segment_overrun();
void exception_invalid_tss();
void exception_segment_not_present();
void exception_stack_segment_fault();
void exception_general_protection();
void exception_page_fault();
void exception_math_fault();
void exception_alignment_check();
void exception_machine_check();
void exception_simd_exception();
void exception_virtualization_exception();
void exception_control_protection_exception();

void idt_set_descriptor(idt_entry_t *t, void *isr, uint8_t flags);
void idt_initialize(idt_entry_t *idt);
void idt_init_exception_handlers(idt_entry_t *idt);

#define INTERRUPT_GATE 0x8E
#define TRAP_GATE 0x8F
#define TASK_GATE 0x85
#define INVALID_GATE 0x0

#define IDT_MAX_DESCRIPTORS 256

void isr_wrapper0();
void isr_wrapper1();
void isr_wrapper2();
void isr_wrapper3();
void isr_wrapper4();
void isr_wrapper5();
void isr_wrapper6();
void isr_wrapper7();
void isr_wrapper8();
void isr_wrapper9();
void isr_wrapper10();
void isr_wrapper11();
void isr_wrapper12();
void isr_wrapper13();
void isr_wrapper14();
void isr_wrapper15();

void isr_handler(int irq);

void idt_init_interrupt_handlers(idt_entry_t *idt);

#endif