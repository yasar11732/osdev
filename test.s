	.file	"test.c"
	.text
	.globl	args_test
	.type	args_test, @function
args_test:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$12, %esp
	pushl	$12
	call	mf
	addl	$16, %esp
	nop
	leave
	ret
	.size	args_test, .-args_test
	.ident	"GCC: (GNU) 11.2.0"
