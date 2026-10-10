gcc -m32 -c kernel.c -o kc.o -ffreestanding -nostdlib -fno-builtin &&
	gcc -m32 -c keyboard.c -o keyboard.o -ffreestanding -nostdlib -fno-builtin &&
	gcc -m32 -c shell.c -o shell.o -ffreestanding -nostdlib -fno-builtin &&
	gcc -m32 -c idt.c -o idt.o -ffreestanding -nostdlib -fno-builtin &&
	gcc -m32 -c timer.c -o timer.o -ffreestanding -nostdlib -fno-builtin
gcc -m32 -c framebuffer.c -o framebuffer.o -ffreestanding -nostdlib -fno-builtin &&
	gcc -m32 -c font8x16.c -o font8x16.o -ffreestanding -nostdlib -fno-builtin &&
	nasm -f elf32 interrupts.asm -o interrupts.o &&
	nasm -f elf32 boot.asm -o kasm.o &&
	ld -m elf_i386 -T linker.ld -o kernel kasm.o kc.o keyboard.o idt.o timer.o interrupts.o framebuffer.o font8x16.o

cp kernel iso/boot/kernel
grub-mkrescue -o catos.iso iso

qemu-system-i386 -cdrom catos.iso
