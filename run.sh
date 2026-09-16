gcc -m32 -c kernel.c -o kc.o -ffreestanding -nostdlib -fno-builtin
gcc -m32 -c keyboard.c -o keyboard.o -ffreestanding -nostdlib -fno-builtin
gcc -m32 -c vga.c -o vga.o -ffreestanding -nostdlib -fno-builtin

ld -m elf_i386 -T linker.ld -o kernel kasm.o kc.o keyboard.o vga.o

qemu-system-i386 -kernel kernel
