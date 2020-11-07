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

kaffenos.elf: boot.o kernel.o linker.ld
	$(CC) $(CFLAGS) -nostdlib -T linker.ld boot.o kernel.o -o kaffenos.elf -lgcc

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

boot.o: boot.asm
	$(ASM) $(AFLAGS) boot.asm -o boot.o

clean:
	rm -rf *.o