section .multiboot
align 8

header_start:

    dd 0xE85250D6
    dd 0
    dd header_end - header_start
    dd -(0xE85250D6 + 0 + (header_end - header_start))

    ; framebuffer tag

    dw 5
    dw 0
    dd 20

    dd 1024
    dd 768
    dd 32

    align 8

    ; end tag

    dw 0
    dw 0
    dd 8

header_end:

section .bss
align 16

stack_bottom:
    resb 16384

stack_top:

section .text
global _start

extern kernel_main

_start:
    cli

    mov esp, stack_top

    ; EBX = multiboot address
    push ebx

    call kernel_main

.hang:
    hlt
    jmp .hang