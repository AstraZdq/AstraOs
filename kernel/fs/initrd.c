#include "initrd.h"

#include "vfs.h"

#include <stdint.h>

extern unsigned char
_binary_build_user_test_elf_start;

extern unsigned char
_binary_build_user_test_elf_end;

#define INITRD_SIZE 0x10000

static uint8_t initrd_data[INITRD_SIZE];

void initrd_initialize()
{
    uint8_t* ptr = initrd_data;

    /*
    =========================
    hello.txt
    =========================
    */

    initrd_file_header_t* header1 =
        (initrd_file_header_t*)ptr;

    const char* hello =
        "Hello from structured initrd!";

    int hello_size = 31;

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

    /*
    =========================
    readme.txt
    =========================
    */

    initrd_file_header_t* header2 =
        (initrd_file_header_t*)ptr;

    const char* readme =
        "Welcome to AstraOS.";

    int readme_size = 21;

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

    /*
    =========================
    test.elf
    =========================
    */

    initrd_file_header_t* header3 =
        (initrd_file_header_t*)ptr;

    uint8_t* elf_data =
        &_binary_build_user_test_elf_start;

    uint32_t elf_size =
    (
        uint32_t)
    &_binary_build_user_test_elf_end
    -
    (uint32_t)
    &_binary_build_user_test_elf_start;

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

    for (i = 0; i < (int)elf_size; i++)
    {
        ptr[i] = elf_data[i];
    }

    ptr += elf_size;

    /*
    =========================
    Mount files into VFS
    =========================
    */

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