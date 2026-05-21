#pragma once

#include <stdint.h>

typedef struct
{
    unsigned char magic[4];

    uint8_t elf_class;

    uint8_t endian;

    uint8_t elf_version;

    uint8_t abi;

    uint8_t padding[8];

    uint16_t type;

    uint16_t machine;

    uint32_t version;

    uint32_t entry;

    uint32_t phoff;

    uint32_t shoff;

    uint32_t flags;

    uint16_t ehsize;

    uint16_t phentsize;

    uint16_t phnum;

    uint16_t shentsize;

    uint16_t shnum;

    uint16_t shstrndx;

} __attribute__((packed)) elf_header_t;

typedef struct
{
    uint32_t type;

    uint32_t offset;

    uint32_t virtual_address;

    uint32_t physical_address;

    uint32_t file_size;

    uint32_t memory_size;

    uint32_t flags;

    uint32_t align;

} __attribute__((packed)) elf_program_header_t;

int elf_validate(void* data);

void elf_load(void* data);