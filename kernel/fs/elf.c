#include "elf.h"

#include "terminal.h"

#include <stdint.h>

static void print_hex(uint32_t value)
{
    char hex[] = "0123456789ABCDEF";

    for (int i = 28; i >= 0; i -= 4)
    {
        terminal_putchar(
            hex[(value >> i) & 0xF]
        );
    }
}

int elf_validate(void* data)
{
    elf_header_t* elf =
        (elf_header_t*)data;

    if (elf->magic[0] != 0x7F)
    {
        return 0;
    }

    if (elf->magic[1] != 'E')
    {
        return 0;
    }

    if (elf->magic[2] != 'L')
    {
        return 0;
    }

    if (elf->magic[3] != 'F')
    {
        return 0;
    }

    return 1;
}

void elf_load(void* data)
{
    elf_header_t* elf =
        (elf_header_t*)data;

    terminal_write(
        "      [+] ELF header validated\n"
    );

    terminal_write(
        "      [+] Entry point: 0x"
    );

    print_hex(elf->entry);

    terminal_write("\n");

    terminal_write(
        "Program headers: "
    );

    print_hex(elf->phnum);

    terminal_write("\n");

    elf_program_header_t* ph =
        (elf_program_header_t*)
    (
        (uint8_t*)data + elf->phoff
    );

    for (int i = 0; i < elf->phnum; i++)
{
    if (ph[i].type != 1)
    {
        continue;
    }

    uint8_t* source =
        (uint8_t*)data + ph[i].offset;

    uint8_t* destination =
        (uint8_t*)ph[i].virtual_address;

    for (uint32_t j = 0; j < ph[i].file_size; j++)
    {
        destination[j] = source[j];
    }

    terminal_write(
        "Segment loaded at: 0x"
    );

    print_hex(
        ph[i].virtual_address
    );

    terminal_write("\n");
}
}