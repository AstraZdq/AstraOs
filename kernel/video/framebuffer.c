#include "framebuffer.h"

#include "font.h"


static uint32_t* framebuffer = 0;

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

    screen_width = width;
    screen_height = height;

    screen_pitch =
        pitch / 4;
}

void framebuffer_clear(
    uint32_t color)
{
    for (uint32_t y = 0;
         y < screen_height;
         y++)
    {
        for (uint32_t x = 0;
             x < screen_width;
             x++)
        {
            framebuffer[
                y * screen_pitch + x
            ] = color;
        }
    }
}

void framebuffer_pixel(
    int x,
    int y,
    uint32_t color)
{
    if (x < 0 || y < 0)
    {
        return;
    }

    if (x >= (int)screen_width)
    {
        return;
    }

    if (y >= (int)screen_height)
    {
        return;
    }

    framebuffer[
        y * screen_pitch + x
    ] = color;
}

void framebuffer_rect(
    int x,
    int y,
    int w,
    int h,
    uint32_t color)
{
    for (int py = 0;
         py < h;
         py++)
    {
        for (int px = 0;
             px < w;
             px++)
        {
            framebuffer_pixel(
                x + px,
                y + py,
                color
            );
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
    uint8_t* glyph =
        font[(int)c];

    for (int py = 0;
         py < 8;
         py++)
    {
        for (int px = 0;
             px < 8;
             px++)
        {
            if (
                glyph[py] &
                (1 << (7 - px))
            )
            {
                framebuffer_pixel(
                    x + px,
                    y + py,
                    fg
                );
            }
            else
            {
                framebuffer_pixel(
                    x + px,
                    y + py,
                    bg
                );
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
    int y
)
{
    if (x < 0 || y < 0)
    {
        return 0;
    }

    if (x >= (int)screen_width)
    {
        return 0;
    }

    if (y >= (int)screen_height)
    {
        return 0;
    }

    return framebuffer[
        y * screen_pitch + x
    ];
}

uint32_t framebuffer_get_width()
{
    return screen_width;
}

uint32_t framebuffer_get_height()
{
    return screen_height;
}