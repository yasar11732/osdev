
.section .text
.global invlpg
.type invlpg,@function
invlpg:
    invlpg 0x4(%esp)
    ret