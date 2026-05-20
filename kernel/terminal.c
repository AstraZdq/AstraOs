#include "terminal.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static size_t terminal_row;
static size_t terminal_column;

static uint8_t terminal_color;

static uint16_t* terminal_buffer = (uint16_t*)0xB8000;

static inline uint8_t vga_entry_color(uint8_t fg, uint8_t bg)
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t)uc | (uint16_t)color << 8;
}

void terminal_clear()
{
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;

            terminal_buffer[index] =
                vga_entry(' ', terminal_color);
        }
    }

    terminal_row = 0;
    terminal_column = 0;
}

void terminal_initialize()
{
    terminal_row = 0;
    terminal_column = 0;

    terminal_color = vga_entry_color(15, 0);

    terminal_clear();
}

void terminal_putchar(char c)
{
    if (c == '\n')
    {
        terminal_column = 0;
        terminal_row++;

        return;
    }

    const size_t index =
        terminal_row * VGA_WIDTH + terminal_column;

    terminal_buffer[index] =
        vga_entry(c, terminal_color);

    terminal_column++;

    if (terminal_column >= VGA_WIDTH)
    {
        terminal_column = 0;
        terminal_row++;
    }

    if (terminal_row >= VGA_HEIGHT)
    {
        terminal_row = 0;
    }
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