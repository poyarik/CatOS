#include "keyboard.h"
#include "vga.h"

#define MAX_INPUT_LEN 256

const char* prompt = "CatOS>";
extern volatile unsigned short* vga;
extern int row;
extern int cursor;

void kmain(void) {
    clear_vga();

    const char* str = "CatOS";
    int base_cur = strlen(prompt);
    unsigned char color = 0x7a;

    int len = strlen(str);

    int row = 12;
    int hidden_row = 0;
    int col = (WIDTH - len) / 2;

    int index = (row * WIDTH) + col;


    for (int i = 0; str[i] != '\0'; i++) {
        vga[index + i] = convert_to_vga(str[i], color);
    }

    delay(1000);

    clear_vga();

    println(prompt);

	char buff[256] = "\0";
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
            hidden_row = 0;

            if (row >= HEIGHT) {
                row = 0;
                clear_vga();
            }
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
