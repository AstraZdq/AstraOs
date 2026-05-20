#include "terminal.h"
#include "shell.h"
#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "keyboard.h"
#include "time.h"
#include "heap.h"
#include "paging.h"
#include "scheduler.h"
#include "task.h"

void task_a()
{
    terminal_write("A");

    sleep(50);
}

void task_b()
{
    terminal_write("B");

    sleep(50);
}

void kernel_main()
{
    terminal_initialize();

    terminal_write("Booting AstraOS...\n\n");

    gdt_initialize();

    pic_initialize();

    idt_initialize();

    pit_initialize(100);

    heap_initialize();

    paging_initialize();

    char* memory = (char*)kmalloc(16);

    memory[0] = 'O';
    memory[1] = 'K';
    memory[2] = '\0';

    terminal_write(memory);
    terminal_write("\n");

    char* test1 = (char*)kmalloc(32);

    test1[0] = 'A';
    test1[1] = '\0';

    terminal_write(test1);
    terminal_write("\n");

    kfree(test1);

    char* test2 = (char*)kmalloc(32);

    test2[0] = 'B';
    test2[1] = '\0';

    terminal_write(test2);
    terminal_write("\n");

    scheduler_initialize();

    task_t* a = task_create(task_a);

    task_t* b = task_create(task_b);

    scheduler_add_task(a);

    scheduler_add_task(b);

    keyboard_initialize();

    shell_initialize();

    __asm__ volatile ("sti");

    scheduler_run();
}