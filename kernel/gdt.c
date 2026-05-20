#include "gdt.h"

#include <stdint.h>

struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern void gdt_flush(uint32_t);

static struct gdt_entry gdt[6];

static struct gdt_ptr gp;

static void gdt_set_gate(
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
        (limit >> 16) & 0x0F;

    gdt[num].granularity |=
        granularity & 0xF0;

    gdt[num].access = access;
}

void gdt_set_tss(
    uint32_t base,
    uint32_t limit)
{
    gdt_set_gate(
        5,
        base,
        limit,
        0x89,
        0x40
    );
}

void gdt_initialize()
{
    gp.limit =
        (sizeof(struct gdt_entry) * 6) - 1;

    gp.base = (uint32_t)&gdt;

    gdt_set_gate(
        0,
        0,
        0,
        0,
        0
    );

    gdt_set_gate(
        1,
        0,
        0xFFFFFFFF,
        0x9A,
        0xCF
    );

    gdt_set_gate(
        2,
        0,
        0xFFFFFFFF,
        0x92,
        0xCF
    );

    gdt_set_gate(
        3,
        0,
        0xFFFFFFFF,
        0xFA,
        0xCF
    );

    gdt_set_gate(
        4,
        0,
        0xFFFFFFFF,
        0xF2,
        0xCF
    );

    gdt_flush((uint32_t)&gp);
}