#include "gdt.h"

#include <stdint.h>

gdt_entry_t gdt[6];

gdt_ptr_t gdt_ptr;

extern void gdt_flush(uint32_t);

void gdt_set_gate(
    int num,
    uint32_t base,
    uint32_t limit,
    uint8_t access,
    uint8_t granularity)
{
    gdt[num].base_low =
        (base & 0xFFFF);

    gdt[num].base_middle =
        (base >> 16) & 0xFF;

    gdt[num].base_high =
        (base >> 24) & 0xFF;

    gdt[num].limit_low =
        (limit & 0xFFFF);

    gdt[num].granularity =
        ((limit >> 16) & 0x0F);

    gdt[num].granularity |=
        (granularity & 0xF0);

    gdt[num].access =
        access;
}

void gdt_set_tss(
    int num,
    uint32_t base,
    uint32_t limit,
    uint8_t access,
    uint8_t granularity)
{
    gdt_set_gate(
        num,
        base,
        limit,
        access,
        granularity
    );
}

void gdt_initialize()
{
    gdt_ptr.limit =
        (sizeof(gdt_entry_t) * 6) - 1;

    gdt_ptr.base =
        (uint32_t)&gdt;

    /*
    =========================
    NULL SEGMENT
    =========================
    */

    gdt_set_gate(
        0,
        0,
        0,
        0,
        0
    );

    /*
    =========================
    KERNEL CODE
    =========================
    */

    gdt_set_gate(
        1,
        0,
        0xFFFFFFFF,
        0x9A,
        0xCF
    );

    /*
    =========================
    KERNEL DATA
    =========================
    */

    gdt_set_gate(
        2,
        0,
        0xFFFFFFFF,
        0x92,
        0xCF
    );

    /*
    =========================
    USER CODE
    =========================
    */

    gdt_set_gate(
        3,
        0,
        0xFFFFFFFF,
        0xFA,
        0xCF
    );

    /*
    =========================
    USER DATA
    =========================
    */

    gdt_set_gate(
        4,
        0,
        0xFFFFFFFF,
        0xF2,
        0xCF
    );

    gdt_flush(
        (uint32_t)&gdt_ptr
    );
}