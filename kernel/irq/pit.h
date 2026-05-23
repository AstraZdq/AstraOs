#pragma once

#include <stdint.h>

#include "context.h"

extern uint64_t timer_ticks;

void pit_initialize(
    uint32_t frequency
);

registers_t* pit_handler(
    registers_t* regs
);