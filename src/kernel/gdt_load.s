
.section .asm

gdtr:
    .word 0
    .long 0

.globl setGdt
.type setGdt, @function

setGdt:
    movl 8(%esp), %eax    /* base */
    movl %eax, gdtr+2
    movw 4(%esp), %ax     /* limit */
    movw %ax, gdtr
    lgdt gdtr
    ret

.global reloadSegments
.type reloadSegments, @function

reloadSegments:
    ljmp $0x08, $next

next:
    movw $0x10, %ax              
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ret