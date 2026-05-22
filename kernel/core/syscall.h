#pragma once

#include <stdint.h>

uint32_t syscall_handler(
    uint32_t syscall_id,
    uint32_t arg1,
    uint32_t arg2,
    uint32_t arg3
);

void syscall_initialize();