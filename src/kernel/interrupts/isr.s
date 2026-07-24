.section .text

.global interrupt_stub_30
.type interrupt_stub_30, @function


interrupt_stub_30:
    push $0x30
    jmp interrupt_handler


    # every push has its own negative equivalent: iret undoes the interrupt, pusha gets undone by popa, our add and push cancel each other out.
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
