section .text

global irq0
global irq1
global irq12

global isr14
global isr128

extern pit_handler
extern keyboard_handler
extern mouse_handler

extern pic_send_eoi
extern page_fault_handler
extern syscall_handler

;
; IRQ0 - PIT
;

irq0:
    pusha

    call pit_handler

    push dword 0
    call pic_send_eoi
    add esp, 4

    popa
    iret

;
; IRQ1 - Keyboard
;

irq1:
    pusha

    call keyboard_handler

    push dword 1
    call pic_send_eoi
    add esp, 4

    popa
    iret

;
; IRQ12 - Mouse
;

irq12:
    pusha

    call mouse_handler

    push dword 12
    call pic_send_eoi
    add esp, 4

    popa
    iret

;
; ISR14 - Page Fault
;

isr14:
    cli

    pusha

    mov eax, cr2
    push eax

    mov eax, [esp + 36]
    push eax

    call page_fault_handler

    add esp, 8

    popa

    add esp, 4

    iret

;
; ISR128 - Syscalls
;

isr128:
    pusha

    push edi
    push esi
    push ebx
    push eax

    call syscall_handler

    add esp, 16

    popa

    iret