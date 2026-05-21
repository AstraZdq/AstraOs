#include "scheduler.h"

#include "process.h"

void scheduler_initialize()
{
    process_initialize();
}

void scheduler_run()
{
    process_switch();
}
