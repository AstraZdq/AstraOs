#include "terminal.h"
#include "shell.h"
void shell_start()
{
    terminal_write("AstraOS Shell\n");
    terminal_write("> ");
}