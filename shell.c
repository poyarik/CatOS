#include "shell.h"
#include "vga.h"

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
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
        clear_vga();
    } else if (strcmp(cmd_buffer, "help") == 0) {
        cmd_help();
    } else if (strcmp(cmd_buffer, "about") == 0) {
        cmd_about();
    } else {
        // Неизвестная команда
    }
}
