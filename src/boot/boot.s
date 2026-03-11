.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16382 #16 KiB
stack_top:

.section .data
.align 8
gdt_start:
    .quad 0x0               
    .quad 0x00CF9A000000FFFF
    .quad 0x00CF92000000FFFF  
gdt_end:

gdt_descriptor:
    .word gdt_end - gdt_start - 1  
    .long gdt_start               

.section .text


.global _start
.type _start, @function
_start:
    mov $stack_top, %esp
    cli
    call terminal_initialize
    call put_debug_msg
    lgdt gdt_descriptor
    call reloadSegments
    call kernel_main


    cli
1:  hlt
    jmp 1b

reloadSegments:
   call put_debug_msg
   ljmp $0x08, $.reload_CS
.reload_CS:
   mov $0x10, %ax          
   mov %ax, %ds
   mov %ax, %es
   mov %ax, %fs
   mov %ax, %gs
   mov %ax, %ss
   ret

.size _start, . - _start#

