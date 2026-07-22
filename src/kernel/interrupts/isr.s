.section .text

.global interrupt_stub_30
.type interrupt_stub_30, @function
.extern testHandler


interrupt_stub_30:
    pusha
    call testHandler
    popa
    iret

.size interrupt_stub_30, . - interrupt_stub_30
