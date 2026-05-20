#include "terminal.h"
#include "shell.h"
void kernel_main()
{
    terminal_initialize();

    terminal_write("Booting AstraOS...\n\n");

    shell_start();

    while (1)
    {
        __asm__ volatile ("hlt");
    }
}