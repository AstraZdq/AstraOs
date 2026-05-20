ISO=astraos.iso
KERNEL=build/astra.bin

C_SOURCES=$(wildcard kernel/*.c)
C_OBJECTS=$(patsubst kernel/%.c,build/%.o,$(C_SOURCES))

all: $(ISO)

build:
	mkdir -p build

build/kernel_entry.o: boot/kernel_entry.asm | build
	nasm -f elf32 boot/kernel_entry.asm -o build/kernel_entry.o

build/gdt_asm.o: kernel/gdt.asm | build
	nasm -f elf32 kernel/gdt.asm -o build/gdt_asm.o

build/isr.o: kernel/isr.asm | build
	nasm -f elf32 kernel/isr.asm -o build/isr.o

build/usermode.o: kernel/usermode.asm | build
	nasm -f elf32 kernel/usermode.asm -o build/usermode.o

build/tss_asm.o: kernel/tss.asm | build
	nasm -f elf32 kernel/tss.asm -o build/tss_asm.o

build/%.o: kernel/%.c | build
	gcc -m32 -ffreestanding -Wall -Wextra -c $< -o $@

$(KERNEL): \
	build/kernel_entry.o \
	$(C_OBJECTS) \
	build/gdt_asm.o \
	build/isr.o \
	build/usermode.o \
	build/tss_asm.o
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