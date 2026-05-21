#pragma once

#include <stdint.h>

typedef struct
{
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;

    uint32_t stack;
    
    int active;

} process_t;

void process_initialize();

int create_process(void (*entry)());

void process_switch();
