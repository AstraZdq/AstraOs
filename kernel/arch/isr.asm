global irq0
global irq1
global isr14
global isr128
extern syscall_handler

extern pit_handler
extern keyboard_handler
extern pic_send_eoi
extern page_fault_handler

irq0:
    pusha

    call pit_handler

    push dword 0
    call pic_send_eoi
    add esp, 4

    popa

    iret

irq1:
    pusha

    call keyboard_handler

    push dword 1
    call pic_send_eoi
    add esp, 4

    popa

    iret

isr14:
    cli

    pusha

    mov eax, [esp + 32]
    push eax
    call page_fault_handler
    add esp, 4

    popa

    add esp, 4

    iret

isr128:
    pusha

    push eax

    call syscall_handler

    add esp, 4

    popa

    iret
