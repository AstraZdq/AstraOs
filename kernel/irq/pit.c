#include "pit.h"
#include "io.h"
#include "scheduler.h"

volatile uint64_t timer_ticks = 0;

void pit_initialize(uint32_t frequency)
{
    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);

    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}

void pit_handler()
{
    timer_ticks++;

    scheduler_run();
}
