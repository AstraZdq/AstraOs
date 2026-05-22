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
#include "process.h"

#include "syscall.h"

#include "vfs.h"
#include "initrd.h"
#include "elf.h"

#include "panic.h"

#include "tss.h"

void kernel_main()
{
    terminal_initialize();

   

    terminal_write("  [*] Initializing GDT...\n");
    gdt_initialize();

    terminal_write("  [*] Initializing TSS...\n");
    tss_initialize();

    terminal_write("  [*] Initializing PIC...\n");
    pic_initialize();

    terminal_write("  [*] Initializing IDT...\n");
    idt_initialize();

    terminal_write("  [*] Setting up paging...\n");
    paging_initialize();

    terminal_write("  [*] Initializing heap...\n");
    heap_initialize();

    terminal_write("  [*] Mounting filesystem...\n");
    vfs_initialize();

    terminal_write("  [*] Loading initrd...\n");
    initrd_initialize();

    terminal_write("  [*] Initializing syscalls...\n");
    syscall_initialize();

    terminal_write("  [*] Initializing process manager...\n");
    process_initialize();

    terminal_write("  [*] Initializing scheduler...\n");
    scheduler_initialize();

    terminal_write("  [*] Starting timer (100 Hz)...\n");
    pit_initialize(100);

    terminal_write("\n  [OK] Kernel boot complete!\n\n");

     terminal_write("\n");
    terminal_write("  ========================================\n");
    terminal_write("    Welcome to AstraOS Kernel\n");
    terminal_write("    x86 32-bit Architecture\n");
    terminal_write("  ========================================\n");
    terminal_write("\n");

    file_t* hello =
        vfs_open("hello.txt");

    if (hello)
    {
        terminal_write("  [+] hello.txt: ");
        terminal_write(
            (char*)hello->data
        );

        terminal_write("\n");
    }

    file_t* readme =
        vfs_open("readme.txt");

    if (readme)
    {
        terminal_write("  [+] readme.txt: ");
        terminal_write(
            (char*)readme->data
        );

        terminal_write("\n");
    }

    file_t* elf_file =
        vfs_open("test.elf");

    if (elf_file)
    {
        terminal_write("\n  [*] ELF Binary Check:\n");
        
        if (elf_validate(
            elf_file->data))
        {
            elf_load(
                elf_file->data
            );
        }
        else
        {
            terminal_write("      [!] ELF validation failed\n");
        }
    }

    terminal_write("\n  [*] Initializing keyboard...\n");
    keyboard_initialize();

    terminal_write("  [*] Starting shell...\n");
    

    terminal_write("\n  Type 'help' for available commands\n");
    terminal_write("  -----------------------------------\n\n");
    shell_initialize();

    

    __asm__ volatile ("sti");

    while (1)
    {
        __asm__ volatile ("hlt");
    }
}