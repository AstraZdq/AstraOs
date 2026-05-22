#include "faults.h"

#include "terminal.h"

#include <stdint.h>

static void print_hex(uint32_t value)
{
    char hex[] = "0123456789ABCDEF";

    for (int i = 28; i >= 0; i -= 4)
    {
        terminal_putchar(hex[(value >> i) & 0xF]);
    }
}

void page_fault_handler(uint32_t error_code)
{
    uint32_t fault_address;

    __asm__ volatile ("mov %%cr2, %0" : "=r" (fault_address));

    terminal_write("\nPAGE FAULT\n");
    terminal_write("Fault address: 0x");
    print_hex(fault_address);
    terminal_write("\nError code: 0x");
    print_hex(error_code);
    terminal_write("\n");

    while (1)
    {
        __asm__ volatile ("hlt");
    }
}
