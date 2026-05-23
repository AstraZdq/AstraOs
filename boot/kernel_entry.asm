section .multiboot
align 8

header_start:

    ;
    ; multiboot2 magic
    ;

    dd 0xe85250d6
    dd 0
    dd header_end - header_start
    dd -(0xe85250d6 + 0 + (header_end - header_start))

    ;
    ; framebuffer request tag
    ;

    align 8

    dd 8
    dd 20

    dd 1024
    dd 768
    dd 32

    ;
    ; end tag
    ;

    align 8

    dd 0
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

    ;
    ; ebx = multiboot info
    ;

    push ebx

    call kernel_main

    add esp, 4

.hang:
    hlt
    jmp .hang