.section .text

.global interrupt_stub_30
.type interrupt_stub_30, @function


interrupt_stub_30:
    push $0x30
    jmp interrupt_handler

.size interrupt_stub_30, . - interrupt_stub_30

.global interrupt_stub_21
.type interrupt_stub_21, @function


interrupt_stub_21:
    push $0x21
    jmp interrupt_handler


    # every push has its own negative equivalent: iret undoes the interrupt, pusha gets undone by popa, our add and push cancel each other out.
.size interrupt_stub_21, . - interrupt_stub_21

.global interrupt_stub_20
.type interrupt_stub_20, @function


interrupt_stub_20:
    push $0x20
    jmp interrupt_handler


    # every push has its own negative equivalent: iret undoes the interrupt, pusha gets undone by popa, our add and push cancel each other out.
.size interrupt_stub_20, . - interrupt_stub_20

.global interrupt_stub_00
.type interrupt_stub_00, @function

interrupt_stub_00:
    push $0x00
    jmp interrupt_handler

.size interrupt_stub_00, . - interrupt_stub_00

.global interrupt_stub_01
.type interrupt_stub_01, @function

interrupt_stub_01:
    push $0x01
    jmp interrupt_handler

.size interrupt_stub_01, . - interrupt_stub_01

.global interrupt_stub_02
.type interrupt_stub_02, @function

interrupt_stub_02:
    push $0x02
    jmp interrupt_handler

.size interrupt_stub_02, . - interrupt_stub_02

.global interrupt_stub_04
.type interrupt_stub_04, @function

interrupt_stub_04:
    push $0x04
    jmp interrupt_handler

.size interrupt_stub_04, . - interrupt_stub_04

.global interrupt_stub_05
.type interrupt_stub_05, @function

interrupt_stub_05:
    push $0x05
    jmp interrupt_handler

.size interrupt_stub_05, . - interrupt_stub_05

.global interrupt_stub_06
.type interrupt_stub_06, @function

interrupt_stub_06:
    push $0x06
    jmp interrupt_handler

.size interrupt_stub_06, . - interrupt_stub_06

.global interrupt_stub_07
.type interrupt_stub_07, @function

interrupt_stub_07:
    push $0x07
    jmp interrupt_handler

.size interrupt_stub_07, . - interrupt_stub_07

.global interrupt_stub_08
.type interrupt_stub_08, @function

interrupt_stub_08:
    push $0x08
    jmp interrupt_handler

.size interrupt_stub_08, . - interrupt_stub_08

.global interrupt_handler
.type interrupt_handler, @function
.extern interrupt_dispatcher

interrupt_handler:
    pusha
    lea 36(%esp), %eax      # interrupt frame pointer
    push %eax               # push to stack
    push 36(%esp)           # vector number
    call interrupt_dispatcher
    add $8, %esp            # undo the two pushes
    popa
    add $4, %esp            # undo the stub's push
    iret                    # undo interrupts save

.size interrupt_handler, . - interrupt_handler
