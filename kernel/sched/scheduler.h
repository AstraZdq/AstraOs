#pragma once

#include "task.h"

void scheduler_initialize();

void scheduler_add_task(task_t* task);

void scheduler_run();
