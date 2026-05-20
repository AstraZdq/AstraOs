#include "scheduler.h"

static task_t* current_task = 0;

void scheduler_initialize()
{
    current_task = 0;
}

void scheduler_add_task(task_t* task)
{
    if (!current_task)
    {
        current_task = task;

        task->next = task;

        return;
    }

    task_t* temp = current_task;

    while (temp->next != current_task)
    {
        temp = temp->next;
    }

    temp->next = task;

    task->next = current_task;
}

void scheduler_schedule()
{
    if (!current_task)
    {
        return;
    }

    void (*task_entry)() =
        (void(*)())current_task->eip;

    task_entry();

    current_task = current_task->next;
}