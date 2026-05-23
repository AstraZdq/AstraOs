#include "scheduler.h"

#include "task.h"

#include "terminal.h"

static task_t* current = 0;

void scheduler_initialize()
{
    current = task_current();

    terminal_write(
        "[scheduler ready]\n"
    );
}

registers_t* scheduler_switch(
    registers_t* regs)
{
    if (!current)
    {
        return regs;
    }

    /*
    save current task
    */

    current->regs = *regs;

    /*
    next task
    */

    current = task_next();

    /*
    restore next task
    */

    return regs;
}