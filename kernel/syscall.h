#pragma once

#include <stdint.h>

void syscall_initialize();

void syscall_handler(uint32_t syscall_number);