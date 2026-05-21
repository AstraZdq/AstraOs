#pragma once

#include <stdint.h>

typedef struct
{
    char name[32];

    uint8_t* data;

    uint32_t size;

} file_t;

void vfs_initialize();

int vfs_create_file(
    const char* name,
    uint8_t* data,
    uint32_t size
);

file_t* vfs_open(const char* name);