#include "keyboard.h"
#include "io.h"
#include "terminal.h"
#include "shell.h"

static char keyboard_map[128] =
{
    0,
    27,
    '1','2','3','4','5','6','7','8','9','0',
    '-','=',
    '\b',
    '\t',
    'q','w','e','r','t','y','u','i','o','p',
    '[',']',
    '\n',
    0,
    'a','s','d','f','g','h','j','k','l',
    ';','\'','`',
    0,
    '\\',
    'z','x','c','v','b','n','m',
    ',', '.', '/',
    0,
    '*',
    0,
    ' ',
};

void keyboard_initialize()
{
}

void keyboard_update()
{
    if (!(inb(0x64) & 1))
    {
        return;
    }

    uint8_t scancode = inb(0x60);

    if (scancode > 57)
    {
        return;
    }

    char c = keyboard_map[scancode];

    if (c)
    {
        shell_input(c);
    }
}