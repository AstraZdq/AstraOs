#include "idt.h"

#include <stddef.h>
#include <stdint.h>

#include "terminal.h"

extern void idt_load(uint32_t);

extern void irq0();
extern void irq1();

extern void isr14();
extern void isr128();

idt_entry_t idt[256];

idt_ptr_t idt_ptr;

static void idt_set_gate(
    uint8_t num,
    uint32_t base,
    uint16_t selector,
    uint8_t flags)
{
    idt[num].base_low =
        base & 0xFFFF;

    idt[num].base_high =
        (base >> 16) & 0xFFFF;

    idt[num].selector =
        selector;

    idt[num].always0 = 0;

    idt[num].flags =
        flags;
}

void idt_initialize()
{
    idt_ptr.limit =
        sizeof(idt_entry_t) * 256 - 1;

    idt_ptr.base =
        (uint32_t)&idt;

    for (int i = 0; i < 256; i++)
    {
        idt_set_gate(
            i,
            0,
            0,
            0
        );
    }

    /*
    =========================
    IRQ TIMER
    =========================
    */

    idt_set_gate(
        32,
        (uint32_t)irq0,
        0x08,
        0x8E
    );

    /*
    =========================
    IRQ KEYBOARD
    =========================
    */

    idt_set_gate(
        33,
        (uint32_t)irq1,
        0x08,
        0x8E
    );

    /*
    =========================
    PAGE FAULT
    =========================
    */

    idt_set_gate(
        14,
        (uint32_t)isr14,
        0x08,
        0x8E
    );

    /*
    =========================
    SYSCALL
    =========================
    */

    idt_set_gate(
        128,
        (uint32_t)isr128,
        0x08,
        0xEE
    );

    idt_load(
        (uint32_t)&idt_ptr
    );

    terminal_write(
        "[*] IDT initialized\n"
    );
}