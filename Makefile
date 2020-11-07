SHELL = /bin/sh

#tools
CC=~/opt/cross/bin/i686-elf-gcc
ASM=nasm

#flags
CFLAGS=-g -std=gnu99 -ffreestanding
AFLAGS=-felf

#files
C_FILES = $(SHELL find sources/ -type f -name "*.c")
A_FILES = $(SHELL find sources/ -type f -name "*.asm")

all:kaffenos.elf

kaffenos.elf: boot.o kernel.o desc_tabs.o gdt.o linker.ld
	$(CC) $(CFLAGS) -nostdlib -T linker.ld boot.o kernel.o desc_tabs.o gdt.o -o kaffenos.elf -lgcc

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

boot.o: boot.asm
	$(ASM) $(AFLAGS) boot.asm -o boot.o

gdt.o: gdt.asm
	$(ASM) $(AFLAGS) gdt.asm -o gdt.o

desc_tabs.o: desc_tabs.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf *.o