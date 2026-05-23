ISO=astraos.iso

KERNEL=build/astra.bin

#
# SOURCES
#

C_SOURCES=$(shell find kernel -name '*.c' | sort)

C_OBJECTS=$(patsubst kernel/%.c,build/%.o,$(C_SOURCES))

ASM_SOURCES= \
	kernel/arch/gdt.asm \
	kernel/arch/idt.asm \
	kernel/arch/isr.asm \
	kernel/arch/tss.asm \
	kernel/core/usermode.asm \
	kernel/sched/switch.asm

ASM_OBJECTS=$(patsubst kernel/%.asm,build/%_asm.o,$(ASM_SOURCES))

#
# BUILD
#

all: $(ISO)

build:
	mkdir -p build

#
# BOOT
#

build/kernel_entry.o: boot/kernel_entry.asm | build
	nasm \
	-f elf32 \
	boot/kernel_entry.asm \
	-o build/kernel_entry.o

#
# ASM
#

build/%_asm.o: kernel/%.asm | build
	mkdir -p $(dir $@)

	nasm \
	-f elf32 \
	$< \
	-o $@

#
# C
#

build/%.o: kernel/%.c | build
	mkdir -p $(dir $@)

	gcc \
	-m32 \
	-ffreestanding \
	-fno-pie \
	-fno-stack-protector \
	-Wall \
	-Wextra \
	-Ikernel \
	-Ikernel/arch \
	-Ikernel/core \
	-Ikernel/irq \
	-Ikernel/mm \
	-Ikernel/sched \
	-Ikernel/fs \
	-Ikernel/sys \
	-Ikernel/video \
	-c $< \
	-o $@

#
# USER ELF
#

build/user/test.o:
	mkdir -p build/user

	gcc \
	-m32 \
	-nostdlib \
	-ffreestanding \
	-fno-pie \
	-c user/test.c \
	-o build/user/test.o

build/user/test.elf: build/user/test.o
	ld \
	-m elf_i386 \
	-Ttext 0x00400000 \
	-o build/user/test.elf \
	build/user/test.o

build/user/test_elf.o: build/user/test.elf
	objcopy \
	-I binary \
	-O elf32-i386 \
	-B i386 \
	build/user/test.elf \
	build/user/test_elf.o

#
# LINK
#

$(KERNEL): \
	build/kernel_entry.o \
	$(C_OBJECTS) \
	$(ASM_OBJECTS) \
	build/user/test_elf.o

	ld \
	-m elf_i386 \
	-T linker/linker.ld \
	-o $(KERNEL) \
	$^

#
# ISO
#

$(ISO): $(KERNEL)
	mkdir -p iso/boot/grub

	cp $(KERNEL) \
	iso/boot/astra.bin

	cp boot/grub/grub.cfg \
	iso/boot/grub/grub.cfg

	grub-mkrescue \
	-o $(ISO) \
	iso

#
# RUN
#

run: $(ISO)
	qemu-system-i386 \
	-m 512M \
	-cdrom $(ISO)

#
# CLEAN
#

clean:
	rm -rf build
	rm -rf iso
	rm -f $(ISO)

#
# REBUILD
#

re: clean all