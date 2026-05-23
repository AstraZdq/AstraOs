#include "framebuffer.h"

#include "font.h"

#include "heap.h"
#include "string.h"

static uint32_t* framebuffer = 0;

static uint32_t* backbuffer = 0;

static uint32_t screen_width = 0;
static uint32_t screen_height = 0;
static uint32_t screen_pitch = 0;

void framebuffer_initialize(
    uint32_t address,
    uint32_t width,
    uint32_t height,
    uint32_t pitch)
{
    framebuffer =
        (uint32_t*)address;

    backbuffer = 0;

    screen_width = width;

    screen_height = height;

    screen_pitch =
        pitch / 4;
}

void framebuffer_clear(
    uint32_t color)
{
    uint32_t* dest = backbuffer ? backbuffer : framebuffer;
    if (!dest) return;

    for (uint32_t y = 0;
         y < screen_height;
         y++)
    {
        for (uint32_t x = 0;
             x < screen_width;
             x++)
        {
            dest[y * screen_pitch + x] = color;
        }
    }
}

void framebuffer_pixel(
    int x,
    int y,
    uint32_t color)
{
    if (x < 0 || y < 0 || x >= (int)screen_width || y >= (int)screen_height)
    {
        return;
    }

    uint32_t* dest = backbuffer ? backbuffer : framebuffer;
    if (!dest) return;

    dest[y * screen_pitch + x] = color;
}

void framebuffer_rect(
    int x,
    int y,
    int w,
    int h,
    uint32_t color)
{
    uint32_t* dest = backbuffer ? backbuffer : framebuffer;
    if (!dest) return;

    for (int py = 0; py < h; py++)
    {
        int cy = y + py;
        if (cy < 0 || cy >= (int)screen_height) continue;

        uint32_t offset = cy * screen_pitch;
        for (int px = 0; px < w; px++)
        {
            int cx = x + px;
            if (cx < 0 || cx >= (int)screen_width) continue;

            dest[offset + cx] = color;
        }
    }
}

void framebuffer_char(
    int x,
    int y,
    char c,
    uint32_t fg,
    uint32_t bg)
{
    uint32_t* dest = backbuffer ? backbuffer : framebuffer;
    if (!dest) return;

    // Guard ASCII range
    if ((int)c < 0 || (int)c >= 128) return;

    uint8_t* glyph = font[(int)c];

    for (int py = 0; py < 8; py++)
    {
        int cy = y + py;
        if (cy < 0 || cy >= (int)screen_height) continue;

        uint32_t offset = cy * screen_pitch;
        uint8_t row = glyph[py];

        for (int px = 0; px < 8; px++)
        {
            int cx = x + px;
            if (cx < 0 || cx >= (int)screen_width) continue;

            if (row & (1 << (7 - px)))
            {
                dest[offset + cx] = fg;
            }
            else
            {
                dest[offset + cx] = bg;
            }
        }
    }
}

void framebuffer_text(
    int x,
    int y,
    const char* str,
    uint32_t fg,
    uint32_t bg)
{
    int offset = 0;

    while (*str)
    {
        framebuffer_char(
            x + offset,
            y,
            *str,
            fg,
            bg
        );

        offset += 8;

        str++;
    }
}

uint32_t framebuffer_get_pixel(
    int x,
    int y)
{
    if (x < 0 || y < 0 || x >= (int)screen_width || y >= (int)screen_height)
    {
        return 0;
    }

    uint32_t* src = backbuffer ? backbuffer : framebuffer;
    if (!src) return 0;

    return src[y * screen_pitch + x];
}

uint32_t framebuffer_get_width()
{
    return screen_width;
}

uint32_t framebuffer_get_height()
{
    return screen_height;
}

void framebuffer_swap()
{
    if (!backbuffer || !framebuffer) return;

    for (uint32_t y = 0; y < screen_height; y++)
    {
        for (uint32_t x = 0; x < screen_width; x++)
        {
            framebuffer[y * screen_pitch + x] = backbuffer[y * screen_pitch + x];
        }
    }
}

void framebuffer_init_backbuffer()
{
    if (screen_width > 0 && screen_height > 0)
    {
        backbuffer = (uint32_t*)kmalloc(screen_width * screen_height * 4);
        if (backbuffer)
        {
            for (uint32_t i = 0; i < screen_width * screen_height; i++)
            {
                backbuffer[i] = 0;
            }
        }
    }
}

void framebuffer_draw_gradient()
{
    uint32_t* dest = backbuffer ? backbuffer : framebuffer;
    if (!dest) return;

    for (uint32_t y = 0; y < screen_height; y++)
    {
        uint32_t r = 0x1A + (0x24 * y / screen_height);
        uint32_t g = 0x1A + (0x12 * y / screen_height);
        uint32_t b = 0x35 + (0x30 * y / screen_height);
        uint32_t color = (r << 16) | (g << 8) | b;

        uint32_t offset = y * screen_pitch;
        for (uint32_t x = 0; x < screen_width; x++)
        {
            dest[offset + x] = color;
        }
    }
}