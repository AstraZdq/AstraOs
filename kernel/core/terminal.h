#pragma once

#include <stddef.h>
#include <stdint.h>

void terminal_initialize();
void terminal_putchar(char c);
void terminal_write(const char* data);
void terminal_clear();
