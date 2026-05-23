#include "pit.h"

#include "io.h"

#include "scheduler.h"

#include "context.h"

uint64_t timer_ticks = 0;

registers_t* pit_handler(
    registers_t* regs)
{
    timer_ticks++;

    scheduler_switch(regs);

    outb(0x20, 0x20);

    return regs;
}

void pit_initialize(
    uint32_t frequency)
{
    uint32_t divisor =
        1193180 / frequency;

    outb(0x43, 0x36);

    outb(0x40,
        divisor & 0xFF);

    outb(0x40,
        (divisor >> 8) & 0xFF);
}