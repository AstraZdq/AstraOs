#include "shell.h"

#include "terminal.h"
#include "keyboard.h"

#include "vfs.h"
#include "elf.h"

#include <stddef.h>

static char shell_buffer[256];
static int shell_index = 0;

static void shell_execute(const char* command)
{
    if (command[0] == '\0')
    {
        return;
    }

    /*
    =========================
    help
    =========================
    */

    if (
        command[0] == 'h' &&
        command[1] == 'e' &&
        command[2] == 'l' &&
        command[3] == 'p' &&
        command[4] == '\0'
    )
    {
        terminal_write("Commands:\n");
        terminal_write("help\n");
        terminal_write("clear\n");
        terminal_write("version\n");
        terminal_write("halt\n");
        terminal_write("run\n");
        return;
    }

    /*
    =========================
    clear
    =========================
    */

    if (
        command[0] == 'c' &&
        command[1] == 'l' &&
        command[2] == 'e' &&
        command[3] == 'a' &&
        command[4] == 'r' &&
        command[5] == '\0'
    )
    {
        terminal_clear();
        return;
    }

    /*
    =========================
    version
    =========================
    */

    if (
        command[0] == 'v' &&
        command[1] == 'e' &&
        command[2] == 'r' &&
        command[3] == 's' &&
        command[4] == 'i' &&
        command[5] == 'o' &&
        command[6] == 'n' &&
        command[7] == '\0'
    )
    {
        terminal_write("AstraOS v0.1\n");
        return;
    }

    /*
    =========================
    halt
    =========================
    */

    if (
        command[0] == 'h' &&
        command[1] == 'a' &&
        command[2] == 'l' &&
        command[3] == 't' &&
        command[4] == '\0'
    )
    {
        terminal_write("System halted.\n");

        while (1)
        {
            __asm__ volatile ("hlt");
        }
    }

    /*
    =========================
    run
    =========================
    */

    if (
    command[0] == 'r' &&
    command[1] == 'u' &&
    command[2] == 'n' &&
    command[3] == '\0'
)
{
    terminal_write("Loading ELF...\n");

    file_t* elf =
        vfs_open("test.elf");

    if (!elf)
    {
        terminal_write("ELF not found\n");
        return;
    }

    if (!elf_validate(elf->data))
    {
        terminal_write("Invalid ELF\n");
        return;
    }

    elf_load(elf->data);

    terminal_write("Executing ELF...\n");

    elf_execute(elf->data);

    return;
}
    /*
    =========================
    unknown
    =========================
    */

    terminal_write("Unknown command\n");
}

void shell_initialize()
{
    shell_index = 0;

    terminal_write("AstraOS Shell\n");
    terminal_write("> ");
}

void shell_input(char c)
{
    /*
    =========================
    ENTER
    =========================
    */

    if (c == '\n')
    {
        terminal_write("\n");

        shell_buffer[shell_index] = '\0';

        shell_execute(shell_buffer);

        shell_index = 0;

        terminal_write("> ");

        return;
    }

    /*
    =========================
    BACKSPACE
    =========================
    */

    if (c == '\b')
    {
        if (shell_index > 0)
        {
            shell_index--;

            terminal_backspace();
        }

        return;
    }

    /*
    =========================
    NORMAL CHAR
    =========================
    */

    if (shell_index < 255)
    {
        shell_buffer[shell_index] = c;

        shell_index++;

        terminal_putchar(c);
    }
}