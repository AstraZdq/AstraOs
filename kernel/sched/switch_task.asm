global switch_task

switch_task:
    pusha

    mov eax, [esp + 36]
    mov [eax], esp

    mov eax, [esp + 40]
    mov esp, [eax]

    popa

    ret