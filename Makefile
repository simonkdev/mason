# Makefile for Mason OS
# Paths and commands are based on the current project structure

CC = i686-elf-gcc
AS = i686-elf-as
GRUB_MKRESCUE = grub-mkrescue
QEMU = qemu-system-i386

SRC_DIR = src
BUILD_DIR = build
ISO_DIR = isodir
ISO_FILE = mason.iso

# Source files
BOOT_S = $(SRC_DIR)/boot/boot.s
GDT_LOAD_S = $(SRC_DIR)/kernel/gdt_load.s
KERNEL_C = $(SRC_DIR)/kernel/kernel.c
HELPERS_C = $(SRC_DIR)/kernel/helpers.c
LINKER_LD = $(SRC_DIR)/linker.ld
GRUB_CFG = $(SRC_DIR)/grub.cfg

# Object files
BOOT_O = $(BUILD_DIR)/boot.o
GDT_LOAD_O = $(BUILD_DIR)/gdt_load.o
KERNEL_O = $(BUILD_DIR)/kernel.o
HELPERS_O = $(BUILD_DIR)/helpers.o
MASON_BIN = $(BUILD_DIR)/mason

.PHONY: all clean build assemble_iso test test-nobuild

all: assemble_iso

clean:
	@echo "Cleaning build and ISO directories..."
	rm -rf $(BUILD_DIR) $(ISO_DIR)

build: $(BUILD_DIR)
	@echo "Building boot and kernel objects..."
	$(AS) $(BOOT_S) -o $(BOOT_O)
	$(AS) $(GDT_LOAD_S) -o $(GDT_LOAD_O)
	$(CC) -c $(KERNEL_C) -o $(KERNEL_O) -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	$(CC) -c $(HELPERS_C) -o $(HELPERS_O) -std=gnu99 -ffreestanding -O2 -Wall -Wextra

	@echo "Linking kernel..."
	$(CC) -T $(LINKER_LD) -o $(MASON_BIN) -ffreestanding -O2 -nostdlib \
		$(BOOT_O) $(KERNEL_O) $(HELPERS_O) $(GDT_LOAD_O) \
		-z max-page-size=0x1000 -lgcc

assemble_iso: build
	@echo "Assembling ISO..."
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(MASON_BIN) $(ISO_DIR)/boot/mason
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $(ISO_FILE) $(ISO_DIR)

test: assemble_iso
	@echo "Running QEMU..."
	$(QEMU) -cdrom $(ISO_FILE)

test-nobuild:
	@echo "Running QEMU with prebuilt kernel..."
	$(QEMU) -kernel $(MASON_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)