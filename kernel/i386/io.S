
.section .text
.align 4
.global outb
.type outb, @function
outb:
	movl	8(%esp), %eax
	movl	4(%esp), %edx
	out    %al,(%dx)
	ret

.align 4
.global outh
.type outh, @function	
outh:
	movl	8(%esp), %eax
	movl	4(%esp), %edx
	out   %ax,(%dx)
	ret

.align 4
.global out
.type out, @function	
out:
	movl	8(%esp), %eax
	movl	4(%esp), %edx
	out    %eax,(%dx)
	ret

.align 4
.global inb
.type inb, @function
inb:
    movl 4(%esp),%edx
    xor %eax,%eax
    in (%dx),%al
    ret

.align 4
.global inh
.type inh, @function
inh:
    movl 4(%esp),%edx
    xor %eax,%eax
    in (%dx),%ax
    ret

.align 4
.global in
.type in, @function
in:
    movl 4(%esp),%edx
    xor %eax,%eax
    in (%dx),%eax
    ret