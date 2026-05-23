#include "mouse.h"

#include "io.h"

#include "framebuffer.h"

static int8_t mouse_cycle = 0;

static int8_t mouse_bytes[3];

static int mouse_pos_x = 400;
static int mouse_pos_y = 300;

/* saved area under cursor (10x10) */
static uint32_t cursor_backup[10 * 10];
static int cursor_saved = 0;
static int prev_x = 0;
static int prev_y = 0;

static void update_cursor();

static void mouse_wait()
{
    while (1)
    {
        if (
            inb(0x64) & 1
        )
        {
            return;
        }
    }
}

static void mouse_write(
    uint8_t data)
{
    while (
        inb(0x64) & 2
    );

    outb(0x64, 0xD4);

    while (
        inb(0x64) & 2
    );

    outb(0x60, data);
}

static uint8_t mouse_read()
{
    mouse_wait();

    return inb(0x60);
}

void mouse_initialize()
{
    /*
    enable auxiliary device
    */

    outb(0x64, 0xA8);

    /*
    enable interrupts
    */

    while (inb(0x64) & 2);
    outb(0x64, 0x20);

    mouse_wait();
    uint8_t status =
        inb(0x60);

    status |= 2;

    while (inb(0x64) & 2);
    outb(0x64, 0x60);

    while (inb(0x64) & 2);
    outb(0x60, status);

    /*
    use default settings
    */

    mouse_write(0xF6);
    mouse_read();

    /*
    enable packet streaming
    */

    mouse_write(0xF4);
    mouse_read();

    /* draw initial cursor */
    update_cursor();
}

static void draw_cursor()
{
    framebuffer_rect(
        mouse_pos_x,
        mouse_pos_y,
        10,
        10,
        0xFFFFFF
    );
}

static void restore_cursor()
{
    if (!cursor_saved)
    {
        return;
    }

    for (int py = 0; py < 10; py++)
    {
        for (int px = 0; px < 10; px++)
        {
            int idx = py * 10 + px;
            framebuffer_pixel(prev_x + px, prev_y + py, cursor_backup[idx]);
        }
    }

    cursor_saved = 0;
}

static void save_cursor_area()
{
    for (int py = 0; py < 10; py++)
    {
        for (int px = 0; px < 10; px++)
        {
            int idx = py * 10 + px;
            cursor_backup[idx] = framebuffer_get_pixel(mouse_pos_x + px, mouse_pos_y + py);
        }
    }

    prev_x = mouse_pos_x;
    prev_y = mouse_pos_y;
    cursor_saved = 1;
}

static void update_cursor()
{
    int width = framebuffer_get_width();
    int height = framebuffer_get_height();

    if (width == 0 || height == 0)
    {
        return;
    }

    restore_cursor();
    save_cursor_area();
    draw_cursor();
}

void mouse_handler()
{
    mouse_wait();
    uint8_t data =
        inb(0x60);

    switch (mouse_cycle)
    {
        case 0:
            /* first packet byte must have bit 3 set */
            if (!(data & 0x08))
            {
                mouse_cycle = 0;
                break;
            }

            mouse_bytes[0] = data;
            mouse_cycle++;
            break;

        case 1:
            mouse_bytes[1] = data;
            mouse_cycle++;
            break;

        case 2:
            mouse_bytes[2] = data;

            mouse_pos_x +=
                mouse_bytes[1];

            mouse_pos_y -=
                mouse_bytes[2];

            if (mouse_pos_x < 0)
            {
                mouse_pos_x = 0;
            }

            if (mouse_pos_y < 0)
            {
                mouse_pos_y = 0;
            }

            int width = framebuffer_get_width();
            int height = framebuffer_get_height();

            if (mouse_pos_x > (width - 10))
            {
                mouse_pos_x = width - 10;
            }

            if (mouse_pos_y > (height - 10))
            {
                mouse_pos_y = height - 10;
            }

            update_cursor();

            mouse_cycle = 0;

            break;
    }
}

int mouse_x()
{
    return mouse_pos_x;
}

int mouse_y()
{
    return mouse_pos_y;
}