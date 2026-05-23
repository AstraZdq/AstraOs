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

#include "task.h"

#include "multiboot.h"

#include "framebuffer.h"

#include "mouse.h"

void kernel_main(
    uint32_t multiboot_addr
)
{
    /*
    =========================
    MULTIBOOT FRAMEBUFFER
    =========================
    */

    multiboot_info_t* mb =
        (multiboot_info_t*)
        multiboot_addr;

    (void)mb;

    multiboot_tag_t* tag =
        (multiboot_tag_t*)
        (multiboot_addr + 8);

    while (tag->type != 0)
    {
        if (tag->type == 8)
        {
            multiboot_tag_framebuffer_t* fb =
                (multiboot_tag_framebuffer_t*)
                tag;

            framebuffer_initialize(
                (uint32_t)
                fb->framebuffer_addr,

                fb->framebuffer_width,
                fb->framebuffer_height,

                fb->framebuffer_pitch
            );

            framebuffer_clear(
                0x101010
            );

            framebuffer_rect(
                100,
                100,
                300,
                200,
                0x00FF00
            );

            framebuffer_text(
                120,
                140,
                "ASTRAOS GUI",
                0xFFFFFF,
                0x00FF00
            );
        }

        tag =
            (multiboot_tag_t*)
            (
                (uint8_t*)tag +
                ((tag->size + 7) & ~7)
            );
    }

    /*
    =========================
    VGA TERMINAL
    =========================
    */

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

    terminal_write("  [*] Initializing task system...\n");
    task_initialize();

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

    terminal_write("\n  [*] Initializing keyboard...\n");
    keyboard_initialize();

    terminal_write("  [*] Initializing mouse...\n");
    mouse_initialize();

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