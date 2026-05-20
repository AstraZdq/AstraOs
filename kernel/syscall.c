#include "syscall.h"

#include "terminal.h"

void syscall_initialize()
{
}

void syscall_handler(uint32_t syscall_number)
{
    switch (syscall_number)
    {
        case 0:
            terminal_write("System Call\n");
            break;

        default:
            terminal_write("Unknown Syscall\n");
            break;
    }
}