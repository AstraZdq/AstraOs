#include "mouse.h"

#include "io.h"

#include "framebuffer.h"

static int8_t mouse_cycle = 0;

static int8_t mouse_bytes[3];

static int mouse_pos_x = 400;
static int mouse_pos_y = 300;

/*
=================================
cursor backup
=================================
*/

static uint32_t cursor_backup[10 * 10];

static int cursor_saved = 0;

static int prev_x = 0;
static int prev_y = 0;

static void update_cursor();

/*
=================================
wait helpers
=================================
*/

static void mouse_wait_input()
{
    while (inb(0x64) & 2);
}

static void mouse_wait_output()
{
    while (!(inb(0x64) & 1));
}

/*
=================================
mouse io
=================================
*/

static void mouse_write(
    uint8_t data)
{
    mouse_wait_input();

    outb(0x64, 0xD4);

    mouse_wait_input();

    outb(0x60, data);
}

static uint8_t mouse_read()
{
    mouse_wait_output();

    return inb(0x60);
}

/*
=================================
init
=================================
*/

void mouse_initialize()
{
    /*
    Disable interrupts temporarily during initialization
    to prevent mouse ACK bytes from triggering interrupts
    and hanging the boot sequence.
    */
    __asm__ volatile ("cli");

    /*
    Flush any pending data in the PS/2 controller output buffer
    to ensure we start in a clean state.
    */
    while (inb(0x64) & 1)
    {
        inb(0x60);
    }

    /*
    enable auxiliary device (mouse port)
    */
    mouse_wait_input();
    outb(0x64, 0xA8);

    /*
    enable interrupts and clock
    */

    mouse_wait_input();

    outb(0x64, 0x20);

    mouse_wait_output();

    uint8_t status =
        inb(0x60);

    status |= 2;
    status &= ~0x20; // Clear bit 5 (disable mouse clock) to enable the mouse!

    mouse_wait_input();

    outb(0x64, 0x60);

    mouse_wait_input();

    outb(0x60, status);

    /*
    default settings
    */

    mouse_write(0xF6);
    mouse_read();

    /*
    enable streaming
    */

    mouse_write(0xF4);
    mouse_read();

    /*
    draw initial cursor
    */

    update_cursor();

    /*
    Re-enable interrupts after mouse initialization is complete.
    */
    __asm__ volatile ("sti");
}

/*
=================================
cursor rendering
=================================
*/

static void draw_cursor()
{
    int width = framebuffer_get_width();
    int height = framebuffer_get_height();

    if (width == 0 || height == 0)
    {
        // Text mode: invert the character attribute at the mouse position
        int tx = mouse_pos_x * 80 / 800;
        int ty = mouse_pos_y * 25 / 600;
        if (tx < 0) tx = 0;
        if (tx >= 80) tx = 79;
        if (ty < 0) ty = 0;
        if (ty >= 25) ty = 24;

        int idx = ty * 80 + tx;
        uint8_t* attr = (uint8_t*)(0xB8000 + 2 * idx + 1);
        *attr ^= 0x70; // Invert background color
    }
    else
    {
        framebuffer_rect(
            mouse_pos_x,
            mouse_pos_y,
            10,
            10,
            0xFFFFFF
        );
    }
}

static void restore_cursor()
{
    if (!cursor_saved)
    {
        return;
    }

    int width = framebuffer_get_width();
    int height = framebuffer_get_height();

    if (width == 0 || height == 0)
    {
        // Text mode: restore the original text attribute
        int idx = prev_y * 80 + prev_x;
        uint8_t* attr = (uint8_t*)(0xB8000 + 2 * idx + 1);
        *attr = cursor_backup[0];
    }
    else
    {
        for (int py = 0; py < 10; py++)
        {
            for (int px = 0; px < 10; px++)
            {
                int idx = py * 10 + px;

                framebuffer_pixel(
                    prev_x + px,
                    prev_y + py,
                    cursor_backup[idx]
                );
            }
        }
    }

    cursor_saved = 0;
}

static void save_cursor_area()
{
    int width = framebuffer_get_width();
    int height = framebuffer_get_height();

    if (width == 0 || height == 0)
    {
        // Text mode: save the text attribute under the cursor
        int tx = mouse_pos_x * 80 / 800;
        int ty = mouse_pos_y * 25 / 600;
        if (tx < 0) tx = 0;
        if (tx >= 80) tx = 79;
        if (ty < 0) ty = 0;
        if (ty >= 25) ty = 24;

        int idx = ty * 80 + tx;
        uint8_t* attr = (uint8_t*)(0xB8000 + 2 * idx + 1);
        cursor_backup[0] = *attr;

        prev_x = tx;
        prev_y = ty;
    }
    else
    {
        for (int py = 0; py < 10; py++)
        {
            for (int px = 0; px < 10; px++)
            {
                int idx = py * 10 + px;

                cursor_backup[idx] =
                    framebuffer_get_pixel(
                        mouse_pos_x + px,
                        mouse_pos_y + py
                    );
            }
        }
        prev_x = mouse_pos_x;
        prev_y = mouse_pos_y;
    }

    cursor_saved = 1;
}

static void update_cursor()
{
    restore_cursor();

    save_cursor_area();

    draw_cursor();
}

/*
=================================
irq12 handler
=================================
*/

void mouse_handler()
{
    /*
    no data available
    */

    if (!(inb(0x64) & 1))
    {
        return;
    }

    uint8_t data =
        inb(0x60);

    /*
    DEBUG
    red square appears if IRQ12 works
    */

    framebuffer_rect(
        0,
        0,
        50,
        50,
        0xFF0000
    );

    switch (mouse_cycle)
    {
        case 0:

            /*
            first byte must contain bit 3
            */

            if (!(data & 0x08))
            {
                mouse_cycle = 0;
                return;
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

            /*
            screen bounds
            */

            if (mouse_pos_x < 0)
            {
                mouse_pos_x = 0;
            }

            if (mouse_pos_y < 0)
            {
                mouse_pos_y = 0;
            }

            int width =
                framebuffer_get_width();

            int height =
                framebuffer_get_height();

            if (width == 0 || height == 0)
            {
                // Virtual bounds for text mode (scaled 800x600)
                if (mouse_pos_x > 799)
                {
                    mouse_pos_x = 799;
                }

                if (mouse_pos_y > 599)
                {
                    mouse_pos_y = 599;
                }
            }
            else
            {
                // Graphics mode bounds
                if (mouse_pos_x > width - 10)
                {
                    mouse_pos_x = width - 10;
                }

                if (mouse_pos_y > height - 10)
                {
                    mouse_pos_y = height - 10;
                }
            }

            /*
            update cursor
            */

            update_cursor();

            mouse_cycle = 0;

            break;
    }
}

/*
=================================
helpers
=================================
*/

int mouse_x()
{
    return mouse_pos_x;
}

int mouse_y()
{
    return mouse_pos_y;
}

int mouse_button_left()
{
    return (mouse_bytes[0] & 1) != 0;
}

int mouse_button_right()
{
    return (mouse_bytes[0] & 2) != 0;
}