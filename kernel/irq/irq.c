#include "irq.h"
#include "pic.h"
#include "keyboard.h"

void irq_initialize()
{
    
}

void irq_handler(registers_t* regs)
{
    switch (regs->int_no)
    {
        case 33:
            keyboard_handler();
            break;
    }
    pic_send_eoi(regs->int_no - 32);
}
