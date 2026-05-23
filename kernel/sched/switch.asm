global task_switch

task_switch:

    ; save old context

    pusha

    mov eax, [esp + 32]
    mov [eax], esp

    ; load new stack

    mov eax, [esp + 36]
    mov esp, [eax]

    ; restore new context

    popa

    ret