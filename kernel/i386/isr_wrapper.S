.section .text

.align 4
.global isr_wrapper0
.type isr_wrapper0, @function
isr_wrapper0:
    pushal
    pushl $0
    call isr_handler
    addl $4,%esp
    popal
    iret

.align 4
.global isr_wrapper1
.type isr_wrapper1, @function
isr_wrapper1:
    pushal
    pushl $1
    call isr_handler
    addl $4,%esp
    popal
    iret

.align 4
.global isr_wrapper2
.type isr_wrapper2, @function
isr_wrapper2:
    pushal
    pushl $2
    call isr_handler
    addl $4,%esp
    popal
    iret

.align 4
.global isr_wrapper3
.type isr_wrapper3, @function
isr_wrapper3:
    pushal
    pushl $3
    call isr_handler
    addl $4,%esp
    popal
    iret

.align 4
.global isr_wrapper4
.type isr_wrapper4, @function
isr_wrapper4:
    pushal
    pushl $4
    call isr_handler
    addl $4,%esp
    popal
    iret

.align 4
.global isr_wrapper5
.type isr_wrapper5, @function
isr_wrapper5:
    pushal
    pushl $5
    call isr_handler
    addl $4,%esp
    popal
    iret

.align 4
.global isr_wrapper6
.type isr_wrapper6, @function
isr_wrapper6:
    pushal
    pushl $6
    call isr_handler
    addl $4,%esp
    popal
    iret

.align 4
.global isr_wrapper7
.type isr_wrapper7, @function
isr_wrapper7:
    pushal
    pushl $7
    call isr_handler
    addl $4,%esp
    popal
    iret

.align 4
.global isr_wrapper8
.type isr_wrapper8, @function
isr_wrapper8:
    pushal
    pushl $8
    call isr_handler
    addl $4,%esp
    popal
    iret

.align 4
.global isr_wrapper9
.type isr_wrapper9, @function
isr_wrapper9:
    pushal
    pushl $9
    call isr_handler
    addl $4,%esp
    popal
    iret

.align 4
.global isr_wrapper10
.type isr_wrapper10, @function
isr_wrapper10:
    pushal
    pushl $10
    call isr_handler
    addl $4,%esp
    popal
    iret

.align 4
.global isr_wrapper11
.type isr_wrapper11, @function
isr_wrapper11:
    pushal
    pushl $11
    call isr_handler
    addl $4,%esp
    popal
    iret

.align 4
.global isr_wrapper12
.type isr_wrapper12, @function
isr_wrapper12:
    pushal
    pushl $12
    call isr_handler
    addl $4,%esp
    popal
    iret

.align 4
.global isr_wrapper13
.type isr_wrapper13, @function
isr_wrapper13:
    pushal
    pushl $13
    call isr_handler
    addl $4,%esp
    popal
    iret

.align 4
.global isr_wrapper14
.type isr_wrapper14, @function
isr_wrapper14:
    pushal
    pushl $14
    call isr_handler
    addl $4,%esp
    popal
    iret

.align 4
.global isr_wrapper15
.type isr_wrapper15, @function
isr_wrapper15:
    pushal
    pushl $15
    call isr_handler
    addl $4,%esp
    popal
    iret
