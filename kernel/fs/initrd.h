#pragma once

#include <stdint.h>

typedef struct
{
    char name[32];

    uint32_t size;

} initrd_file_header_t;

void initrd_initialize();