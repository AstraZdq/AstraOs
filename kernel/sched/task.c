#include "task.h"

#include "terminal.h"

static task_t tasks[16];

static int task_count = 0;

static int next_pid = 1;

static task_t* current_task = 0;

void task_initialize()
{
    task_t* kernel_task =
        &tasks[0];

    kernel_task->pid = 0;

    kernel_task->next =
        kernel_task;

    current_task =
        kernel_task;

    task_count = 1;

    terminal_write(
        "[task system ready]\n"
    );
}

task_t* task_create(
    uint32_t entry)
{
    if (task_count >= 16)
    {
        return 0;
    }

    task_t* task =
        &tasks[task_count];

    task->pid = next_pid++;

    /*
    =========================
    stack setup
    =========================
    */

    uint32_t stack_top =
        0x600000 +
        (task_count * 0x4000);

    registers_t* regs =
        (registers_t*)
        (stack_top - sizeof(registers_t));

    /*
    fake interrupt frame
    */

    regs->eax = 0;
    regs->ebx = 0;
    regs->ecx = 0;
    regs->edx = 0;

    regs->esi = 0;
    regs->edi = 0;
    regs->ebp = 0;

    regs->ds = 0x10;

    regs->eip = entry;
    regs->cs = 0x08;

    regs->eflags = 0x202;

    regs->esp = (uint32_t)regs;
    regs->ss = 0x10;

    task->regs = *regs;

    task->next = current_task;

    task_t* last =
        current_task;

    while (last->next != current_task)
    {
        last = last->next;
    }

    last->next = task;

    task_count++;

    terminal_write(
        "[task created]\n"
    );

    return task;
}

task_t* task_current()
{
    return current_task;
}

task_t* task_next()
{
    current_task =
        current_task->next;

    return current_task;
}