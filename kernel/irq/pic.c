#include "pic.h"
#include "io.h"

void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8)
    {
        outb(0xA0, 0x20);
    }

    outb(0x20, 0x20);
}

void pic_initialize()
{
    /*
    starts initialization
    */

    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    /*
    remap
    */

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    /*
    cascading
    */

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    /*
    environment info
    */

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    /*
    unmask:
    IRQ0  timer
    IRQ1  keyboard
    IRQ2  cascade
    IRQ12 mouse
    */

    outb(0x21, 0xF8);

    outb(0xA1, 0xEF);
}