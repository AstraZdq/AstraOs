#pragma once

#include "context.h"

void scheduler_initialize();

registers_t* scheduler_switch(
    registers_t* regs
);