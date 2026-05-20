ISO=astraos.iso
KERNEL=build/astra.bin

C_SOURCES=$(wildcard kernel/*.c)
C_OBJECTS=$(patsubst kernel/%.c,build/%.o,$(C_SOURCES))

ASM_SOURCES=$(wildcard kernel/*.asm)
ASM_OBJECTS=$(patsubst kernel/%.asm,build/%.o,$(ASM_SOURCES))

all: $(ISO)

build:
	mkdir -p build

build/kernel_entry.o: boot/kernel_entry.asm | build
	nasm -f elf32 $< -o $@

build/%.o: kernel/%.c | build
	gcc -m32 -ffreestanding -Wall -Wextra -c $< -o $@

build/%.o: kernel/%.asm | build
	nasm -f elf32 $< -o $@

$(KERNEL): build/kernel_entry.o $(C_OBJECTS) $(ASM_OBJECTS)
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