#include "terminal.h"
#include "shell.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "keyboard.h"
#include "time.h"
#include "heap.h"

void kernel_main()
{
    terminal_initialize();

    terminal_write("Booting AstraOS...\n\n");

    gdt_initialize();

    pic_initialize();

    idt_initialize();

    pit_initialize(100);

    heap_initialize();

    char* memory = (char*)kmalloc(16);

    memory[0] = 'O';
    memory[1] = 'K';
    memory[2] = '\0';

    terminal_write(memory);
    terminal_write("\n");

    keyboard_initialize();

    shell_initialize();

    __asm__ volatile ("sti");

    while (1)
    {
        __asm__ volatile ("hlt");
    }
}