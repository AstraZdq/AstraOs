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

static inline void serial_write_char(char c)
{
    /* COM1 port: 0x3F8 (data), 0x3FD (line status) */
    /* Wait for transmitter to be empty (bit 5 of line status) */
    unsigned char status;
    
    do {
        __asm__ volatile ("inb $0x3FD, %0" : "=a" (status));
    } while ((status & 0x20) == 0);
    
    /* Send byte to COM1 data port */
    __asm__ volatile ("outb %0, $0x3F8" : : "a" (c));
}

static void terminal_scroll()
{
    /* Move all lines up by one */
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            size_t src_index = (y + 1) * VGA_WIDTH + x;
            size_t dst_index = y * VGA_WIDTH + x;
            terminal_buffer[dst_index] = terminal_buffer[src_index];
        }
    }

    /* Clear the last line */
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
        size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        terminal_buffer[index] = vga_entry(' ', terminal_color);
    }

    /* Keep cursor on last line */
    terminal_row = VGA_HEIGHT - 1;
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

    const size_t index =
        terminal_row * VGA_WIDTH + terminal_column;

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

void terminal_write(const char* data)
{
    size_t i = 0;

    while (data[i] != '\0')
    {
        terminal_putchar(data[i]);
        i++;
    }
}
