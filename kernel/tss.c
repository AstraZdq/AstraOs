#include "tss.h"

#include <stdint.h>

struct tss_entry
{
    uint32_t prev_tss;

    uint32_t esp0;
    uint32_t ss0;

    uint32_t unused[23];

} __attribute__((packed));

static struct tss_entry tss;

extern void tss_flush();

extern void gdt_set_tss(
    uint32_t base,
    uint32_t limit
);

void tss_set_stack(uint32_t stack)
{
    tss.esp0 = stack;
}

void tss_initialize()
{
    uint32_t base =
        (uint32_t)&tss;

    uint32_t limit =
        base + sizeof(tss);

    tss.ss0 = 0x10;

    tss.esp0 = 0;

    gdt_set_tss(base, limit);

    tss_flush();
}