#pragma once

#include "registers.h"

void irq_initialize();
void irq_handler(registers_t* regs);
