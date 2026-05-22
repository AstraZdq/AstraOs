#include "initrd.h"

#include "vfs.h"

#include <stdint.h>

static uint8_t initrd_data[1024];

static uint8_t fake_elf[] =
{
    0x7F, 'E', 'L', 'F',

    1,
    1,
    1,
    0,

    0,0,0,0,0,0,0,0,

    2,0,

    3,0,

    1,0,0,0,

    0x00,0x10,0x00,0x00,

    0x34,0x00,0x00,0x00,

    0,0,0,0,

    0,0,0,0,

    52,0,

    32,0,

    1,0,

    0,0,

    0,0,

    0,0,

    1,0,0,0,

    0x54,0x00,0x00,0x00,

    0x00,0x00,0x40,0x00,

    0x00,0x00,0x40,0x00,

    0x10,0x00,0x00,0x00,

    0x10,0x00,0x00,0x00,

    5,0,0,0,

    0x00,0x00,0x40,0x00
};

void initrd_initialize()
{
    uint8_t* ptr = initrd_data;

    initrd_file_header_t* header1 =
        (initrd_file_header_t*)ptr;

    const char* hello =
        "Hello from structured initrd!";

    int hello_size = sizeof(
    "Hello from structured initrd!"
    );

    int i = 0;

    while ("hello.txt"[i])
    {
        header1->name[i] =
            "hello.txt"[i];

        i++;
    }

    header1->name[i] = 0;

    header1->size = hello_size;

    ptr += sizeof(initrd_file_header_t);

    for (i = 0; i < hello_size; i++)
    {
        ptr[i] = hello[i];
    }

    ptr += hello_size;

    initrd_file_header_t* header2 =
        (initrd_file_header_t*)ptr;

    const char* readme =
        "Welcome to AstraOS.";

    int readme_size = sizeof(
    "Welcome to AstraOS."
    );

    i = 0;

    while ("readme.txt"[i])
    {
        header2->name[i] =
            "readme.txt"[i];

        i++;
    }

    header2->name[i] = 0;

    header2->size = readme_size;

    ptr += sizeof(initrd_file_header_t);

    for (i = 0; i < readme_size; i++)
    {
        ptr[i] = readme[i];
    }

    ptr += readme_size;

    initrd_file_header_t* header3 =
        (initrd_file_header_t*)ptr;

    int elf_size = sizeof(fake_elf);

    i = 0;

    while ("test.elf"[i])
    {
        header3->name[i] =
            "test.elf"[i];

        i++;
    }

    header3->name[i] = 0;

    header3->size = elf_size;

    ptr += sizeof(initrd_file_header_t);

    for (i = 0; i < elf_size; i++)
    {
        ptr[i] = fake_elf[i];
    }

    ptr += elf_size;

    ptr = initrd_data;

    for (int file = 0; file < 3; file++)
    {
        initrd_file_header_t* header =
            (initrd_file_header_t*)ptr;

        ptr += sizeof(initrd_file_header_t);

        vfs_create_file(
            header->name,
            ptr,
            header->size
        );

        ptr += header->size;
    }
}
