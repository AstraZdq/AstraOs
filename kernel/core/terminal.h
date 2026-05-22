#pragma once

#include <stddef.h>
#include <stdint.h>

void terminal_initialize();

void terminal_clear();

void terminal_putchar(char c);

void terminal_write(const char* data);

void terminal_backspace();

void terminal_write_hex(uint32_t value);