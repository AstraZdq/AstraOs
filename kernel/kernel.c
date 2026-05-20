#include "terminal.h"
#include "shell.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "keyboard.h"
#include "time.h"


void kernel_main()
{
    terminal_initialize();

    terminal_write("Booting AstraOS...\n\n");

    gdt_initialize();

    pic_initialize();

    idt_initialize();

    pit_initialize(100);

    keyboard_initialize();

    shell_initialize();

    __asm__ volatile ("sti");

    while (1)
    {
        terminal_write(".");

        sleep(100);

        __asm__ volatile ("hlt");
    }
}