	.file	"splinlock.c"
	.text
	.globl	wait_lock
	.type	wait_lock, @function
wait_lock:
	movl	$0, %ecx
	movl	$1, %edx
.L2:
	movl	%ecx, %eax
	lock cmpxchgl	%edx, mallocLocked
	jne	.L2
	lock orl	$0, (%esp)
	ret
	.size	wait_lock, .-wait_lock
	.globl	mallocLocked
	.section	.bss
	.align 4
	.type	mallocLocked, @object
	.size	mallocLocked, 4
mallocLocked:
	.zero	4
	.ident	"GCC: (GNU) 11.2.0"
