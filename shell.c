#include "shell.h"
#include "keyboard.h"
#include "vga.h"

#define MAX_INPUT_LEN 256

extern int row;
extern int cursor;

const char* prompt = "CatOS>";
int base_cur;

int hidden_row = 0;

char buff[512] = "";
int buff_len = 0;


int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

void process_input(char ch){
	base_cur = strlen(prompt);
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

void run_shell(void) {
	while (1) {
		char ch = keyboard_getchar();
		process_input(ch);
	}
}

// Отдельные функции под каждую команду
static void cmd_help(void) {
    // Вывод списка доступных команд
}

static void cmd_about(void) {
    println("This is CatOS!!!\n");
}

void shell_execute(const char* cmd_buffer) {
    if (cmd_buffer[0] == '\0') {
        return; // Пустой ввод
    }

    if (strcmp(cmd_buffer, "clear") == 0) {
		row = 0;
		cursor = 0;
        clear_vga();
    } else if (strcmp(cmd_buffer, "help") == 0) {
        cmd_help();
    } else if (strcmp(cmd_buffer, "about") == 0) {
        cmd_about();
    } else {
        println("Unknown command!\n");
    }
}
