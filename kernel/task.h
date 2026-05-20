#pragma once

#include <stdint.h>

typedef struct task
{
    uint32_t eip;

    struct task* next;

} task_t;

task_t* task_create(void (*entry)());