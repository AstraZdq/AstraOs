#include "time.h"
#include "pit.h"

void sleep(uint64_t ticks)
{
    uint64_t target = timer_ticks + ticks;

    while (timer_ticks < target)
    {
        __asm__ volatile ("hlt");
    }
}
