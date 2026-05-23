#pragma once

#include <stdint.h>

typedef struct
{
    uint32_t total_size;
    uint32_t reserved;

} multiboot_info_t;

typedef struct
{
    uint32_t type;
    uint32_t size;

} multiboot_tag_t;

typedef struct
{
    uint32_t type;
    uint32_t size;

    uint64_t framebuffer_addr;

    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;

    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;

    uint16_t reserved;

} multiboot_tag_framebuffer_t;