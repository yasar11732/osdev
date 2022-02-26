
.section .rodata

exception_message_divide_error: .asciz "Divide Error"
exception_message_debug_exception: .asciz "Debug Exception"
exception_message_nmi_interrupt: .asciz "NMI Interrupt"
exception_message_breakpoint: .asciz "Breakpoint"
exception_message_overflow: .asciz "Overflow"
exception_message_bound: .asciz "Bound Range Exceeded"
exception_message_invalid_opcode: .asciz "Invalid Opcode"
exception_message_device_not_available: .asciz "Device Not Available"
exception_message_double_fault: .asciz "Double Fault"
exception_message_coprocessor_segment_overrun: .asciz "Coprocessor Segment Overrun"
exception_message_invalid_tss: .asciz "Invalid TSS"
exception_message_segment_not_present: .asciz "Segment Not Present"
exception_message_stack_segment_fault: .asciz "Stack Segment Fault"
exception_message_general_protection: .asciz "General Protection"
exception_message_page_fault: .asciz "Page Fault"
exception_message_math_fault: .asciz "Math Error"
exception_message_alignment_check: .asciz "Alignment Check"
exception_message_machine_check: .asciz "Machine Check"
exception_message_simd_exception: .asciz "Simd Exception"
exception_message_virtualization_exception: .asciz "Virtualization Exception"
exception_message_control_protection_exception: .asciz "Control Protection Exception"

.section .text

.align 4
.global exception_divide_error
.type exception_divide_error, @function
exception_divide_error:
    push $exception_message_divide_error
    jmp halt_forever

.align 4
.global exception_debug_exception
.type exception_debug_exception, @function
exception_debug_exception:
    iret

.align 4
.global exception_nmi_interrupt
.type exception_nmi_interrupt, @function
exception_nmi_interrupt:
    push $exception_message_nmi_interrupt
    jmp halt_forever

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
    push $exception_message_bound
    jmp halt_forever

.align 4
.global exception_invalid_opcode
.type exception_invalid_opcode, @function
exception_invalid_opcode:
    push $exception_message_invalid_opcode
    jmp halt_forever

.align 4
.global exception_device_not_available
.type exception_device_not_available, @function
exception_device_not_available:
    push $exception_message_device_not_available
    jmp halt_forever

.align 4
.global exception_double_fault
.type exception_double_fault, @function
exception_double_fault:
    push $exception_message_double_fault
    jmp halt_forever

.align 4
.global exception_coprocessor_segment_overrun
.type exception_coprocessor_segment_overrun, @function
exception_coprocessor_segment_overrun:
    push $exception_message_coprocessor_segment_overrun
    jmp halt_forever

.align 4
.global exception_invalid_tss
.type exception_invalid_tss, @function
exception_invalid_tss:
    push $exception_message_invalid_tss
    jmp halt_forever

.align 4
.global exception_segment_not_present
.type exception_segment_not_present, @function
exception_segment_not_present:
    push $exception_message_segment_not_present
    jmp halt_forever

.align 4
.global exception_stack_segment_fault
.type exception_stack_segment_fault, @function
exception_stack_segment_fault:
    push $exception_message_stack_segment_fault
    jmp halt_forever

.align 4
.global exception_general_protection
.type exception_general_protection, @function
exception_general_protection:
    push $exception_message_general_protection
    jmp halt_forever

.align 4
.global exception_page_fault
.type exception_page_fault, @function
exception_page_fault:
    push $exception_message_page_fault
    jmp halt_forever

.align 4
.global exception_math_fault
.type exception_math_fault, @function
exception_math_fault:
    push $exception_message_math_fault
    jmp halt_forever

.align 4
.global exception_alignment_check
.type exception_alignment_check, @function
exception_alignment_check:
    push $exception_message_alignment_check
    jmp halt_forever

.align 4
.global exception_machine_check
.type exception_machine_check, @function
exception_machine_check:
    push $exception_message_machine_check
    jmp halt_forever

.align 4
.global exception_simd_exception
.type exception_simd_exception, @function
exception_simd_exception:
    push $exception_message_simd_exception
    jmp halt_forever

.align 4
.global exception_virtualization_exception
.type exception_virtualization_exception, @function
exception_virtualization_exception:
    push $exception_message_virtualization_exception
    jmp halt_forever

.align 4
.global exception_control_protection_exception
.type exception_control_protection_exception, @function
exception_control_protection_exception:
    push $exception_message_control_protection_exception
    jmp halt_forever


halt_forever:
    call terminal_writestring
.align 4
.global null_exception_handler
.type null_exception_handler, @function
null_exception_handler:
    cli
    hlt