#pragma once

#include <stdint.h>

void framebuffer_initialize(
    uint32_t address,
    uint32_t width,
    uint32_t height,
    uint32_t pitch
);

void framebuffer_clear(
    uint32_t color
);

void framebuffer_pixel(
    int x,
    int y,
    uint32_t color
);

void framebuffer_rect(
    int x,
    int y,
    int w,
    int h,
    uint32_t color
);

uint32_t framebuffer_get_pixel(
    int x,
    int y
);

uint32_t framebuffer_get_width();

uint32_t framebuffer_get_height();

void framebuffer_char(
    int x,
    int y,
    char c,
    uint32_t fg,
    uint32_t bg
);

void framebuffer_text(
    int x,
    int y,
    const char* str,
    uint32_t fg,
    uint32_t bg
);