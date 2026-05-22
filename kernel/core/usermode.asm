global enter_user_mode

enter_user_mode:

    mov eax, [esp + 4]

    mov ecx, 0x23
    mov ds, cx
    mov es, cx
    mov fs, cx
    mov gs, cx

    push 0x23
    push 0x408000

    pushf

    push 0x1B
    push eax

    sti

    iret
