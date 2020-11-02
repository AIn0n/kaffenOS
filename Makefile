SHELL = /bin/sh

CC=~/opt/cross/bin/i686-elf-gcc
FLAGS=-g -std=gnu99 -ffreestanding

all:kaffenos.elf

kaffenos.elf: boot.o kernel.o linker.ld
	$(CC) $(FLAGS) -nostdlib -T linker.ld boot.o kernel.o -o kaffenos.elf -lgcc

kernel.o: kernel.c
	$(CC) $(FLAGS) -c kernel.c -o kernel.o

boot.o: boot.s
	$(CC) $(FLAGS) -c boot.s -o boot.o

clean:
	rm -rf *.o