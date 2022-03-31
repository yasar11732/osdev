.section .text

.align 4
.global exception_divide_error
.type exception_divide_error, @function
exception_divide_error:
    pushl $0
    pushal
    pushl $0
    call exception_handler

.align 4
.global exception_debug_exception
.type exception_debug_exception, @function
exception_debug_exception:
    iret

.align 4
.global exception_nmi_interrupt
.type exception_nmi_interrupt, @function
exception_nmi_interrupt:
    pushl $0
    pushal
    pushl $2
    call exception_handler

.align 4
.global exception_breakpoint
.type exception_breakpoint, @function
exception_breakpoint:
    iret

.align 4
.global exception_overflow
.type exception_overflow, @function
exception_overflow:
    iret

.align 4
.global exception_bound
.type exception_bound, @function
exception_bound:
    pushl $0
    pushal
    pushl $5
    call exception_handler

.align 4
.global exception_invalid_opcode
.type exception_invalid_opcode, @function
exception_invalid_opcode:
    pushl $0
    pushal
    pushl $6
    call exception_handler

.align 4
.global exception_device_not_available
.type exception_device_not_available, @function
exception_device_not_available:
    pushl $0
    pushal
    pushl $7
    call exception_handler

.align 4
.global exception_double_fault
.type exception_double_fault, @function
exception_double_fault:
    // pushl $0
    pushal
    pushl $8
    call exception_handler

.align 4
.global exception_coprocessor_segment_overrun
.type exception_coprocessor_segment_overrun, @function
exception_coprocessor_segment_overrun:
    pushl $0
    pushal
    pushl $9
    call exception_handler

.align 4
.global exception_invalid_tss
.type exception_invalid_tss, @function
exception_invalid_tss:
    // pushl $
    pushal
    pushl $0xA
    call exception_handler

.align 4
.global exception_segment_not_present
.type exception_segment_not_present, @function
exception_segment_not_present:
    // pushl $
    pushal
    pushl $0xB
    call exception_handler

.align 4
.global exception_stack_segment_fault
.type exception_stack_segment_fault, @function
exception_stack_segment_fault:
    // pushl $0
    pushal
    pushl $0XC
    call exception_handler

.align 4
.global exception_general_protection
.type exception_general_protection, @function
exception_general_protection:
    // pushl $0
    pushal
    pushl $0xD
    call exception_handler

.align 4
.global exception_page_fault
.type exception_page_fault, @function
exception_page_fault:
    // pushl $0
    pushal
    pushl $0xE
    call exception_handler

.align 4
.global exception_math_fault
.type exception_math_fault, @function
exception_math_fault:
    pushl $0
    pushal
    pushl $0x10
    call exception_handler

.align 4
.global exception_alignment_check
.type exception_alignment_check, @function
exception_alignment_check:
    pushl $0
    pushal
    pushl $0x11
    call exception_handler

.align 4
.global exception_machine_check
.type exception_machine_check, @function
exception_machine_check:
    pushl $0
    pushal
    pushl $0x12
    call exception_handler

.align 4
.global exception_simd_exception
.type exception_simd_exception, @function
exception_simd_exception:
    pushl $0
    pushal
    pushl $0x13
    call exception_handler

.align 4
.global exception_virtualization_exception
.type exception_virtualization_exception, @function
exception_virtualization_exception:
    pushl $0
    pushal
    pushl $0x14
    call exception_handler

.align 4
.global exception_control_protection_exception
.type exception_control_protection_exception, @function
exception_control_protection_exception:
    pushl $0
    pushal
    pushl $0x15
    call exception_handler

.align 4
.global null_exception_handler
.type null_exception_handler, @function
null_exception_handler:
    cli
    hlt