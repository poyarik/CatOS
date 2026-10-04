#include "keyboard.h"
#include "io.h"

#define KEYBOARD_DATA_PORT   0x60
#define KEYBOARD_STATUS_PORT 0x64

// Таблица перевода скан-кодов в ASCII
static const char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0,  '\\',
  'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0, '*',   0, ' '
};

char now = 0;

char keyboard_getchar(void) {
    while (now == 0) {
        __asm__ __volatile__("hlt");
    }

	char ch = now;
	now = 0;

    if (ch < 0x80) {
        return keyboard_map[ch];
    }

    return 0;
}

void keyboard_handler_c(void) {
    char scancode = inb(0x60); // 1. Обязательно вычитываем скан-код

	now = scancode;

    outb(0x20, 0x20); // 3. ОБЯЗАТЕЛЬНО отправляем EOI в PIC!
}
