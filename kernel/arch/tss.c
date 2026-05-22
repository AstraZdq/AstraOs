#include "tss.h"

#include <stdint.h>

tss_entry_t tss;

extern void tss_flush();

void write_tss(
    uint32_t ss0,
    uint32_t esp0)
{
    uint32_t base =
        (uint32_t)&tss;

    uint32_t limit =
        base + sizeof(tss_entry_t);

    extern void gdt_set_tss(
        int num,
        uint32_t base,
        uint32_t limit,
        uint8_t access,
        uint8_t gran);

    gdt_set_tss(
        5,
        base,
        limit,
        0xE9,
        0x00
    );

    tss.ss0 = ss0;
    tss.esp0 = esp0;

    tss.cs = 0x0b;
    tss.ss =
    tss.ds =
    tss.es =
    tss.fs =
    tss.gs = 0x13;
}

void tss_initialize()
{
    write_tss(
        0x10,
        0x1f0000
    );

    tss_flush();
}