#include "vfs.h"

#define MAX_FILES 16

static file_t files[MAX_FILES];

static int file_count = 0;

static int strcmp(
    const char* a,
    const char* b
)
{
    while (*a && *b)
    {
        if (*a != *b)
        {
            return 0;
        }

        a++;
        b++;
    }

    return *a == *b;
}

static void strcpy(
    char* dst,
    const char* src
)
{
    while (*src)
    {
        *dst++ = *src++;
    }

    *dst = 0;
}

void vfs_initialize()
{
    file_count = 0;
}

int vfs_create_file(
    const char* name,
    uint8_t* data,
    uint32_t size
)
{
    if (file_count >= MAX_FILES)
    {
        return -1;
    }

    strcpy(
        files[file_count].name,
        name
    );

    files[file_count].data = data;

    files[file_count].size = size;

    file_count++;

    return 0;
}

file_t* vfs_open(const char* name)
{
    for (int i = 0; i < file_count; i++)
    {
        if (strcmp(files[i].name, name))
        {
            return &files[i];
        }
    }

    return 0;
}