#include "process.h"

#define MAX_PROCESSES 4

static process_t processes[MAX_PROCESSES];
static int current_process = 0;

void process_initialize()
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        processes[i].active = 0;
    }
}

int create_process(void (*entry)())
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        if(!processes[i].active)
        {
            processes[i].active = 1;

            processes[i].eip =
                (uint32_t)entry;

            processes[i].stack =
                0x300000 + (i * 0x2000);

            processes[i].esp =
                processes[i].stack + 0x2000;

            processes[i].ebp =
                processes[i].esp;
            
                return i;
        }
    }

    return -1;
}

void process_switch()
{
    current_process++;

    if (current_process >= MAX_PROCESSES)
    {
        current_process = 0;
    }

    while (!processes[current_process].active)
    {
        current_process++;

        if (current_process >= MAX_PROCESSES)
        {
            current_process = 0;
        }
    }

    void (*entry)() =
        (void(*)())processes[current_process].eip;

    entry();
}
