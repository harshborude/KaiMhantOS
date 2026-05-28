global idt_flush
idt_flush:
    mov eax, [esp+4]
    lidt [eax]
    ret

; ISR0: Divide by Zero
global isr0
extern isr0_handler
isr0:
    pusha
    call isr0_handler
    popa
    iret

; IRQ1: Keyboard
global irq1
extern irq1_handler
irq1:
    pusha
    call irq1_handler
    popa
    iret

; Generic Dummy ISR
global dummy_isr
dummy_isr:
    iret
