#include "shell.h"
#include "terminal.h"

#define SHELL_BUFFER_SIZE 256

static char shell_buffer[SHELL_BUFFER_SIZE];
static int shell_index = 0;

static int string_compare(const char* a, const char* b)
{
    int i = 0;

    while (a[i] && b[i])
    {
        if (a[i] != b[i])
        {
            return 0;
        }

        i++;
    }

    return a[i] == b[i];
}

static void shell_prompt()
{
    terminal_write("\n> ");
}

static void shell_execute()
{
    shell_buffer[shell_index] = '\0';

    if (string_compare(shell_buffer, "help"))
    {
        terminal_write("\nCommands:");
        terminal_write("\nhelp");
        terminal_write("\nclear");
        terminal_write("\nversion");
        terminal_write("\nhalt");
    }
    else if (string_compare(shell_buffer, "clear"))
    {
        terminal_clear();
    }
    else if (string_compare(shell_buffer, "version"))
    {
        terminal_write("\nAstraOS v0.0.1");
    }
    else if (string_compare(shell_buffer, "halt"))
    {
        terminal_write("\nSystem halted");

        while (1)
        {
            __asm__ volatile ("hlt");
        }
    }
    else if (shell_index > 0)
    {
        terminal_write("\nUnknown command");
    }

    shell_index = 0;

    shell_prompt();
}

void shell_initialize()
{
    terminal_write("AstraOS Shell");
    shell_prompt();
}

void shell_input(char c)
{
    if (c == '\n')
    {
        shell_execute();
        return;
    }

    if (c == '\b')
    {
        if (shell_index > 0)
        {
            shell_index--;
        }

        return;
    }

    if (shell_index >= SHELL_BUFFER_SIZE - 1)
    {
        return;
    }

    shell_buffer[shell_index] = c;
    shell_index++;

    terminal_putchar(c);
}
