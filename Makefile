shell = /bin/sh

#directiories
BUILD_DIR ?= build/
SRC_DIR ?= sources/

#tools
CC=~/opt/cross/bin/i686-elf-gcc
ASM=nasm

#flags
CFLAGS=-g -std=gnu99 -ffreestanding
AFLAGS=-felf

#files
FILES := $(shell find $(SRC_DIRS) -name *.c -or -name *.asm)
LINKER=linker.ld

#objects
OBJS := $(FILES:%=$(BUILD_DIR)/%.o)

#kernel
all:$(BUILD_DIR)/kaffenos.elf

$(BUILD_DIR)/kaffenos.elf: $(LINKER) $(OBJS)
	$(CC) $(CFLAGS) -nostdlib -T linker.ld $(OBJS) -o $@ -lgcc

#C language
$(BUILD_DIR)/%.c.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

#Assembly
$(BUILD_DIR)/%.asm.o: %.asm
	$(ASM) $(AFLAGS) $^ -o $@

clean:
	rm -r $(BUILD_DIR)/$(SRC_DIR)/*