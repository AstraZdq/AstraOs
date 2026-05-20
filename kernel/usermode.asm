global enter_user_mode

enter_user_mode:
    cli

    mov ax, 0x23

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp

    push 0x23
    push eax

    pushf

    push 0x1B
    push user_mode_start

    iret

user_mode_start:

    mov eax, 0

    int 0x80

.loop:
    jmp .loop