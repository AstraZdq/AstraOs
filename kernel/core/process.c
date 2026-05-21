#include "process.h"

#include "terminal.h"

#define MAX_PROCESSES 4

static void (*processes[MAX_PROCESSES])();

static int process_count = 0;

static int current_process = 0;

void process_initialize()
{
    process_count = 0;

    current_process = 0;
}

int create_process(void (*entry)())
{
    if (process_count >= MAX_PROCESSES)
    {
        return -1;
    }

    processes[process_count] = entry;

    process_count++;

    return process_count - 1;
}

void process_switch()
{
    if (process_count == 0)
    {
        return;
    }

    processes[current_process]();

    current_process++;

    if (current_process >= process_count)
    {
        current_process = 0;
    }
}