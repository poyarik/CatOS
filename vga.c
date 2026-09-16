#include "vga.h"

volatile unsigned short* vga = (unsigned short*)0xB8000;
int cursor = 0;
int row = 0;

int strlen(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void delay(volatile unsigned long int count) {
    while (count--) {
        for (volatile int i = 0; i < 500000; i++) {
            __asm__ __volatile__("nop");
        }
    }
}

unsigned short convert_to_vga(char ch, unsigned char clr) {
    return (unsigned short)ch | ((unsigned short)clr << 8);
}

void clear_vga(void) {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        vga[i] = convert_to_vga(' ', DEF_CLR);
    }
}

void type_vga(int row, int cursor, char ch) {
    int index = row * WIDTH + cursor;
    vga[index] = convert_to_vga(ch, DEF_CLR);
}

void print(char ch) {
	int index = row * WIDTH + cursor;
    vga[index] = convert_to_vga(ch, DEF_CLR);
}

void println(char* prompt) {
    for (int i = 0; i < strlen(prompt); i++) {
        type_vga(row, i, prompt[i]);
		
		if (prompt[i] == '\n') {
			cursor = 0;
			row++;
		}

		else if (cursor == WIDTH) {
			cursor = 0;
			row++;
		} else {
			cursor++;
		}
    }
}

