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

char keyboard_getchar(void) {
    while ((inb(KEYBOARD_STATUS_PORT) & 1) == 0) {
        // Ожидание
    }

    unsigned char scancode = inb(KEYBOARD_DATA_PORT);

    // Если скан-код меньше 0x80 — это нажатие (Key Press)
    if (scancode < 0x80) {
        return keyboard_map[scancode];
    }

	// Возвращаем EOI
	outb(0x20, 0x20);

    return 0; // Игнорируем отпускания клавиш (Key Release)
}
