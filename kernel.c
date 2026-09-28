#include "keyboard.h"
#include "vga.h"
#include "shell.h"

#define MAX_INPUT_LEN 256

const char* prompt = "CatOS>";
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
    int base_cur = strlen(prompt);
    unsigned char color = 0x8F;

    int len = strlen(str);

	row = 12;
    int hidden_row = 0;
    int col = (WIDTH - len) / 2;

    int index = (row * WIDTH) + col;


    for (int i = 0; str[i] != '\0'; i++) {
        vga[index + i] = convert_to_vga(str[i], color);
    }

    delay(4000);

    clear_vga();

	row = 0;
    println(prompt);
	cursor = base_cur;

	char buff[512] = "";
	int buff_len = 0;

    while (1) {
        char ch = keyboard_getchar();
        if (ch == '\b') {
            if (cursor > base_cur) {
                cursor--;
				buff[buff_len] = '\0';
				buff_len--;
                type_vga(row, cursor, ' ');
                hidden_row = (cursor + 1) / WIDTH;
            }
        } 
        else if (ch == '\n') {
            row = row + hidden_row + 1;
			cursor = 0;
            hidden_row = 0;

            if (row >= HEIGHT) {
                row = 0;
                clear_vga();
            }

			shell_execute(buff);
			buff[0] = '\0';
			buff_len = 0;

        	println(prompt);
        } 
        else if (ch != 0 && buff_len < MAX_INPUT_LEN) {
            type_vga(row, cursor++, ch);
			buff[buff_len] = ch;
			buff[buff_len + 1] = '\0';
			buff_len++;

            if (cursor > WIDTH) {
                hidden_row = (cursor + 1) / WIDTH;
            }
        }
    }
}
