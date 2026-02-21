.section .text

.global _gdt_load
.type _gdt_load, @function

_gdt_load:
    mov 4(%esp), %eax      # pointer to GDTR struct
    lgdt (%eax)            # load GDT

    # Reload data segments
    mov $0x00, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    # Far jump to reload CS
    ljmp $0x00, $flush

flush:
    ret

.size _gdt_load, . - _gdt_load#