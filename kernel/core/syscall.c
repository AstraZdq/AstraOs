#include "syscall.h"

#include "terminal.h"

#include <stdint.h>

uint32_t syscall_handler(
    uint32_t syscall_id,
    uint32_t arg1,
    uint32_t arg2,
    uint32_t arg3)
{
    (void)arg2;
    (void)arg3;

    switch (syscall_id)
    {
        /*
        =========================
        syscall 1 -> print
        =========================
        */

        case 1:
        {
            terminal_write(
                (char*)arg1
            );

            return 0;
        }

        /*
        =========================
        syscall 2 -> exit
        =========================
        */

        case 2:
        {
            terminal_write(
                "[process exited]\n"
            );

            while (1)
            {
                __asm__ volatile ("hlt");
            }

            return 0;
        }

        /*
        =========================
        unknown syscall
        =========================
        */

        default:
        {
            terminal_write(
                "[unknown syscall]\n"
            );

            return 0;
        }
    }
}

void syscall_initialize()
{
    terminal_write(
        "[syscalls ready]\n"
    );
}