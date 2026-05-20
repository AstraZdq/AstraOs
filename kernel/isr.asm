global irq0
global irq1
global isr14
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

    call page_fault_handler

    popa

    iret