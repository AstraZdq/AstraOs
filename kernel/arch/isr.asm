global irq0
global irq1

global isr14
global isr128

extern pit_handler
extern keyboard_handler
extern page_fault_handler
extern syscall_handler

extern pic_send_eoi

;
; =========================
; IRQ0 - PIT
; =========================
;

irq0:

    cli

    push ds
    push es
    push fs
    push gs

    pusha

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ;
    ; pass registers_t*
    ;

    push esp
    call pit_handler

    ;
    ; scheduler returns new regs ptr in eax
    ;

    add esp, 4

    ;
    ; switch stack to new task
    ;

    mov esp, eax

    ;
    ; PIC EOI
    ;

    push dword 0
    call pic_send_eoi
    add esp, 4

    ;
    ; restore new task context
    ;

    popa

    pop gs
    pop fs
    pop es
    pop ds

    sti

    iret

;
; =========================
; IRQ1 - Keyboard
; =========================
;

irq1:

    cli

    push ds
    push es
    push fs
    push gs

    pusha

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call keyboard_handler

    push dword 1
    call pic_send_eoi
    add esp, 4

    popa

    pop gs
    pop fs
    pop es
    pop ds

    sti

    iret

;
; =========================
; ISR14 - PAGE FAULT
; =========================
;

isr14:

    cli

    push ds
    push es
    push fs
    push gs

    pusha

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, [esp + 48]

    push eax
    call page_fault_handler
    add esp, 4

    popa

    pop gs
    pop fs
    pop es
    pop ds

    add esp, 4

    sti

    iret

;
; =========================
; ISR128 - SYSCALL
; =========================
;

isr128:

    cli

    push ds
    push es
    push fs
    push gs

    pusha

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push edi
    push ecx
    push ebx
    push eax

    call syscall_handler

    add esp, 16

    popa

    pop gs
    pop fs
    pop es
    pop ds

    sti

    iret