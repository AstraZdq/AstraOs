#pragma once

#include <stdint.h>

void pit_initialize(uint32_t frequency);
extern volatile uint64_t timer_ticks;
