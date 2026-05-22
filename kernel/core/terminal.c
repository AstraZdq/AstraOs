#include "terminal.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static size_t terminal_row;
static size_t terminal_column;

static uint8_t terminal_color;

static uint16_t* terminal_buffer =
    (uint16_t*)0xB8000;

static inline uint8_t vga_entry_color(
    uint8_t fg,
    uint8_t bg)
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(
    unsigned char uc,
    uint8_t color)
{
    return (uint16_t)uc |
           (uint16_t)color << 8;
}

static inline void serial_write_char(char c)
{
    unsigned char status;

    do
    {
        __asm__ volatile (
            "inb $0x3FD, %0"
            : "=a"(status)
        );

    } while ((status & 0x20) == 0);

    __asm__ volatile (
        "outb %0, $0x3F8"
        :
        : "a"(c)
    );
}

static void terminal_scroll()
{
    for (size_t y = 0;
         y < VGA_HEIGHT - 1;
         y++)
    {
        for (size_t x = 0;
             x < VGA_WIDTH;
             x++)
        {
            size_t src =
                (y + 1) * VGA_WIDTH + x;

            size_t dst =
                y * VGA_WIDTH + x;

            terminal_buffer[dst] =
                terminal_buffer[src];
        }
    }

    for (size_t x = 0;
         x < VGA_WIDTH;
         x++)
    {
        size_t index =
            (VGA_HEIGHT - 1)
            * VGA_WIDTH + x;

        terminal_buffer[index] =
            vga_entry(
                ' ',
                terminal_color
            );
    }

    terminal_row = VGA_HEIGHT - 1;
    terminal_column = 0;
}

void terminal_clear()
{
    for (size_t y = 0;
         y < VGA_HEIGHT;
         y++)
    {
        for (size_t x = 0;
             x < VGA_WIDTH;
             x++)
        {
            const size_t index =
                y * VGA_WIDTH + x;

            terminal_buffer[index] =
                vga_entry(
                    ' ',
                    terminal_color
                );
        }
    }

    terminal_row = 0;
    terminal_column = 0;
}

void terminal_initialize()
{
    terminal_row = 0;
    terminal_column = 0;

    terminal_color =
        vga_entry_color(15, 0);

    terminal_clear();
}

void terminal_putchar(char c)
{
    serial_write_char(c);

    if (c == '\n')
    {
        terminal_column = 0;
        terminal_row++;

        if (terminal_row >= VGA_HEIGHT)
        {
            terminal_scroll();
        }

        return;
    }

    if (c == '\r')
    {
        terminal_column = 0;
        return;
    }

    const size_t index =
        terminal_row * VGA_WIDTH
        + terminal_column;

    terminal_buffer[index] =
        vga_entry(c, terminal_color);

    terminal_column++;

    if (terminal_column >= VGA_WIDTH)
    {
        terminal_column = 0;
        terminal_row++;

        if (terminal_row >= VGA_HEIGHT)
        {
            terminal_scroll();
        }
    }
}

void terminal_backspace()
{
    if (terminal_column == 0)
    {
        if (terminal_row == 0)
        {
            return;
        }

        terminal_row--;
        terminal_column =
            VGA_WIDTH - 1;
    }
    else
    {
        terminal_column--;
    }

    const size_t index =
        terminal_row * VGA_WIDTH
        + terminal_column;

    terminal_buffer[index] =
        vga_entry(
            ' ',
            terminal_color
        );
}

void terminal_write(const char* data)
{
    size_t i = 0;

    while (data[i] != '\0')
    {
        terminal_putchar(data[i]);
        i++;
    }
}

void terminal_write_hex(uint32_t value)
{
    char hex[] =
        "0123456789ABCDEF";

    terminal_write("0x");

    for (int i = 28;
         i >= 0;
         i -= 4)
    {
        char c =
            hex[(value >> i) & 0xF];

        terminal_putchar(c);
    }
}