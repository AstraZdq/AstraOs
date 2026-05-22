#include "elf.h"

#include "terminal.h"

#include <stdint.h>

static void memory_copy(
    uint8_t* src,
    uint8_t* dst,
    uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        dst[i] = src[i];
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

    terminal_write("ELF detected\n");

    terminal_write("Entry point: ");
    terminal_write_hex(elf->entry);
    terminal_write("\n");

    terminal_write("Program headers: ");
    terminal_write_hex(elf->phnum);
    terminal_write("\n");

    elf_program_header_t* ph =
        (elf_program_header_t*)
        ((uint8_t*)data + elf->phoff);

    for (int i = 0; i < elf->phnum; i++)
    {
        if (ph[i].type != 1)
        {
            continue;
        }

        uint8_t* src =
            (uint8_t*)data + ph[i].offset;

        uint8_t* dst =
            (uint8_t*)ph[i].virtual_address;

        memory_copy(
            src,
            dst,
            ph[i].file_size
        );

        terminal_write("Segment loaded at: ");
        terminal_write_hex(ph[i].virtual_address);
        terminal_write("\n");
    }

    terminal_write("ELF ready for execution\n");
}

void elf_execute(void* data)
{
    elf_header_t* elf =
        (elf_header_t*)data;

    terminal_write("Launching ELF...\n");

    void (*entry)() =
        (void(*)())elf->entry;

    entry();
}