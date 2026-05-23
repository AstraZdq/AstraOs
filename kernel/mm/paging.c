#include "paging.h"

#include <stdint.h>

static uint32_t page_directory[1024]
__attribute__((aligned(4096)));

static uint32_t first_page_table[1024]
__attribute__((aligned(4096)));

static uint32_t second_page_table[1024]
__attribute__((aligned(4096)));

static uint32_t dev_page_table[1024]
__attribute__((aligned(4096)));

void paging_initialize()
{
    for (uint32_t i = 0; i < 1024; i++)
    {
        first_page_table[i] =
            (i * 0x1000) | 7;
    }

    for (uint32_t i = 0; i < 1024; i++)
    {
        second_page_table[i] =
            ((i + 1024) * 0x1000) | 7;
    }

    for (uint32_t i = 0; i < 1024; i++)
    {
        if (i == 0 || i == 1)
        {
            continue;
        }

        if (page_directory[i] == 0)
        {
            page_directory[i] = 0x00000002;
        }
    }

    page_directory[0] =
        ((uint32_t)first_page_table) | 7;

    page_directory[1] =
        ((uint32_t)second_page_table) | 7;

    __asm__ volatile (
        "mov %0, %%cr3"
        :
        : "r"(page_directory)
    );

    uint32_t cr0;

    __asm__ volatile (
        "mov %%cr0, %0"
        : "=r"(cr0)
    );

    cr0 |= 0x80000000;

    __asm__ volatile (
        "mov %0, %%cr0"
        :
        : "r"(cr0)
    );
}

void paging_map_device(uint32_t physical_addr, uint32_t size)
{
    // Align address to 4MB boundary
    uint32_t align_addr = physical_addr & ~0x3FFFFF;
    uint32_t offset = physical_addr - align_addr;
    uint32_t total_size = size + offset;
    
    uint32_t num_pages = (total_size + 4095) / 4096;
    if (num_pages > 1024) num_pages = 1024;

    for (uint32_t i = 0; i < 1024; i++)
    {
        if (i < num_pages)
        {
            dev_page_table[i] =
                (align_addr + (i * 0x1000)) | 7;
        }
        else
        {
            dev_page_table[i] = 0;
        }
    }

    uint32_t dir_idx = align_addr / 0x400000;
    page_directory[dir_idx] =
        ((uint32_t)dev_page_table) | 7;
}