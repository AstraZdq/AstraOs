#include "task.h"

#include "heap.h"

task_t* task_create(void (*entry)())
{
    task_t* task =
        (task_t*)kmalloc(sizeof(task_t));

    task->eip = (uint32_t)entry;

    task->next = 0;

    return task;
}