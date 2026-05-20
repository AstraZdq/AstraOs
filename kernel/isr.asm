global irq0
global irq1
extern pit_handler
extern keyboard_handler
extern pic_send_eoi

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