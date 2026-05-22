#include "panic.h"

#include "terminal.h"

void kernel_panic(
    const char* message
)
{
    terminal_write(
        "\n\n"
    );

    terminal_write(
        "========================================\n"
    );

    terminal_write(
        "            KERNEL PANIC\n"
    );

    terminal_write(
        "========================================\n\n"
    );

    terminal_write(
        "ERROR: "
    );

    terminal_write(
        message
    );

    terminal_write(
        "\n\nSystem halted."
        "\n"
    );

    __asm__ volatile ("cli");

    while (1)
    {
        __asm__ volatile ("hlt");
    }
}