#include "faults.h"

#include "terminal.h"

void page_fault_handler()
{
    terminal_write("\nPAGE FAULT\n");

    while (1)
    {
        __asm__ volatile ("hlt");
    }
}
