#include "terminal.h"
#include "shell.h"
#include "keyboard.h"
void kernel_main()
{
    terminal_initialize();

    terminal_write("Booting AstraOS...\n\n");

    keyboard_initialize();
    
    shell_start();

    while (1)
    {
        keyboard_update();
    }
}