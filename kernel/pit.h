#pragma once

#include <stdint.h>

extern volatile uint64_t timer_ticks;

void pit_initialize(uint32_t frequency);
void pit_handler();