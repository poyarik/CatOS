#include "vga.h"

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

void print(char ch) {
	int index = row * WIDTH + cursor;
    
}

void println(char* prompt) {
    for (int i = 0; i < strlen(prompt); i++) {
		if (prompt[i] == '\n') {
			cursor = 0;
			row++;
		} else {
        	type_vga(row, i, prompt[i]);
			if (cursor == WIDTH) {
				cursor = 0;
				row++;
			} else {
				cursor++;
			}
		}
    }
}

