#pragma once

#include <stdint.h>

#include "context.h"

typedef struct task
{
    /*
    =========================
    task info
    =========================
    */

    int pid;

    /*
    =========================
    saved registers
    =========================
    */

    registers_t regs;

    /*
    =========================
    kernel stack
    =========================
    */

    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;

    /*
    =========================
    paging
    =========================
    */

    uint32_t page_directory;

    /*
    =========================
    linked list
    =========================
    */

    struct task* next;

} task_t;

void task_initialize();

task_t* task_create(
    uint32_t entry
);

task_t* task_current();

task_t* task_next();