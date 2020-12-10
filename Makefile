shell = /bin/sh

#directiories
BUILD_DIR ?= build/
SRC_DIR ?= sources/

#tools
CC=~/opt/cross/bin/i686-elf-gcc
ASM=nasm
MKDIR_P ?= mkdir -p

#flags
CFLAGS=-g -O0 -std=gnu99 -ffreestanding -Wall
AFLAGS=-felf

CFLAGS+= -Isources/descriptors_tables
CFLAGS+= -Isources/terminal
CFLAGS+= -Isources/interrupts
CFLAGS+= -Isources/misc
CFLAGS+= -Isources/drivers/pit
CFLAGS+= -Isources/drivers/keyboard
CFLAGS+= -Isources/math
CFLAGS+= -Isources/string

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
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $^ -o $@

#Assembly
$(BUILD_DIR)/%.asm.o: %.asm
	$(MKDIR_P) $(dir $@)
	$(ASM) $(AFLAGS) $^ -o $@

clean:
	rm -r $(BUILD_DIR)/$(SRC_DIR)/*