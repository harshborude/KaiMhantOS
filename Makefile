AS = nasm
CC = gcc
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-pie -mgeneral-regs-only
LDFLAGS = -m32 -T linker.ld -nostdlib -ffreestanding -O2 -no-pie -Wl,--build-id=none

SRC_DIR = src
BUILD_DIR = build

OBJS = $(BUILD_DIR)/boot.o \
       $(BUILD_DIR)/interrupts.o \
       $(BUILD_DIR)/kernel.o \
       $(BUILD_DIR)/vga.o \
       $(BUILD_DIR)/keyboard.o \
       $(BUILD_DIR)/idt.o \
       $(BUILD_DIR)/pic.o \
       $(BUILD_DIR)/isr.o

ISO = kaimhantos.iso
KERNEL = $(BUILD_DIR)/kernel.bin

.PHONY: all clean iso run

all: $(KERNEL)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm | $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(KERNEL): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

iso: $(KERNEL)
	mkdir -p $(BUILD_DIR)/isodir/boot/grub
	cp $(KERNEL) $(BUILD_DIR)/isodir/boot/myos.bin
	echo 'menuentry "KaiMhantOS" {' > $(BUILD_DIR)/isodir/boot/grub/grub.cfg
	echo '	multiboot /boot/myos.bin' >> $(BUILD_DIR)/isodir/boot/grub/grub.cfg
	echo '}' >> $(BUILD_DIR)/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(BUILD_DIR)/isodir

run: iso
	qemu-system-i386 -cdrom $(ISO) -boot d

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(ISO)
