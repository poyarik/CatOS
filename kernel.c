#include "keyboard.h"
#include "vga.h"
#include "shell.h"
#include "idt.h"


extern char* prompt;
extern volatile unsigned short* vga;
extern int row;
extern int cursor;

// Запись байта в указанный I/O порт
static inline void outb(unsigned short port, unsigned char val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Чтение байта из I/O порта
static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void kmain(void) {
    clear_vga();

	// Отключение курсора
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);

    const char* str = "CatOS, Copyright (c) 2026 Poyarik. Rights Are Not Reserved.";
    unsigned char color = 0x8F;

    int len = strlen(str);

    int index = (12 * WIDTH) + (WIDTH - len) / 2;


    for (int i = 0; str[i] != '\0'; i++) {
        vga[index + i] = convert_to_vga(str[i], color);
    }

    delay(4000);

    clear_vga();

	idt_init();

    println(prompt);
	
	run_shell();
}
