#include "idt.h"

struct idt_entry
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern void irq1();
extern void irq0();
extern void isr14();
extern void isr128();

static struct idt_entry idt[256];
static struct idt_ptr idtp;

static void idt_set_gate(
    uint8_t num,
    uint32_t base,
    uint16_t selector,
    uint8_t flags)
{
    idt[num].offset_low = base & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
    idt[num].offset_high = (base >> 16) & 0xFFFF;
}

static inline void idt_load(struct idt_ptr* ptr)
{
    __asm__ volatile ("lidtl (%0)" : : "r"(ptr));
}

void idt_initialize()
{
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < 256; i++)
    {
        idt_set_gate(i, 0, 0, 0);
    }

    idt_set_gate(
        32,
        (uint32_t)irq0,
        0x08,
        0x8E
    );

    idt_set_gate(
        33,
        (uint32_t)irq1,
        0x08,
        0x8E
    );

    idt_set_gate(
        14,
        (uint32_t)isr14,
        0x08,
        0x8E
    );
    idt_set_gate(
    128,
    (uint32_t)isr128,
    0x08,
    0xEE
);

    idt_load(&idtp);
}