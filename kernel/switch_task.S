
.section .text
.global switch_task
.type switch_task,@function
.align 4

// void switch_task(struct task *next)
switch_task:
    push %ebx
    push %esi
    push %edi
    push %ebp

    mov current_task,%edi
    mov %esp,0x0(%edi)

    mov 0x14(%esp), %esi // esi = next_task
    mov %esi,current_task // current_task = next_task
    mov (%esi), %esp // esp = next_task->esp
    mov 0x4(%esi),%eax // eax = next_task->cr3
    mov %cr3,%ebx // ebx = cr3

    cmp %eax,%ebx
    je 1f
    mov %eax,%cr3
1:
    pop %ebp
    pop %edi
    pop %esi
    pop %ebx
    ret