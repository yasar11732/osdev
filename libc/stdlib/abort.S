.section .text
.global abort
.type abort, @function
abort:
    cli
    hlt