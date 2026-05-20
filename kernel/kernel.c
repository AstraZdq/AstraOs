#include "terminal.h"
#include "shell.h"
#include "pit.h"

void kernel_main()
{
    terminal_initialize();

    terminal_write("Booting AstraOS...\n\n");

    pit_initialize(100);

    shell_initialize();

    while (1)
    {
        __asm__ volatile ("hlt");
    }
}