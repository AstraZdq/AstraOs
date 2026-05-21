#pragma once

#include <stdint.h>

void tss_initialize();

void tss_set_stack(uint32_t stack);
