ISO=astraos.iso
KERNEL=build/astra.bin

# Find all C sources recursively in kernel/
C_SOURCES=$(shell find kernel -name '*.c' | sort)
C_OBJECTS=$(patsubst kernel/%.c,build/%.o,$(C_SOURCES))

all: $(ISO)

build:
	mkdir -p build

build/kernel_entry.o: boot/kernel_entry.asm | build
	nasm -f elf32 boot/kernel_entry.asm -o build/kernel_entry.o

# Assembly sources (located in subdirs)
build/gdt_asm.o: kernel/arch/gdt.asm | build
	nasm -f elf32 kernel/arch/gdt.asm -o build/gdt_asm.o

build/isr.o: kernel/arch/isr.asm | build
	nasm -f elf32 kernel/arch/isr.asm -o build/isr.o

build/usermode.o: kernel/core/usermode.asm | build
	nasm -f elf32 kernel/core/usermode.asm -o build/usermode.o

build/sched/switch_task.o: kernel/sched/switch_task.asm | build
	nasm -f elf32 kernel/sched/switch_task.asm -o build/sched/switch_task.o

build/tss_asm.o: kernel/arch/tss.asm | build
	nasm -f elf32 kernel/arch/tss.asm -o build/tss_asm.o

# Compile C sources; ensure output directory exists and include kernel subdirs
build/%.o: kernel/%.c | build
	mkdir -p $(dir $@)
	gcc -m32 -ffreestanding -Wall -Wextra \
		-Ikernel -Ikernel/arch -Ikernel/core -Ikernel/irq -Ikernel/mm -Ikernel/sched -Ikernel/fs -Ikernel/sys \
		-c $< -o $@

$(KERNEL): \
	build/kernel_entry.o \
	$(C_OBJECTS) \
	build/gdt_asm.o \
	build/isr.o \
	build/usermode.o \
	build/tss_asm.o \
	build/sched/switch_task.o
	ld -m elf_i386 -T linker/linker.ld -o $@ $^

$(ISO): $(KERNEL)
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/astra.bin
	cp boot/grub/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf build iso $(ISO)

re: clean all