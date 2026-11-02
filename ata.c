#include "ata.h"
#include "io.h"
#include <stdint.h>

int read_sector(uint32_t lba, uint8_t *target_buffer) {
	while (inb(0x1F7) & 0x80);
	
	outb(0x1F2, 1);
	outb(0x1F3, (uint8_t)lba);
	outb(0x1F4, (uint8_t)(lba >> 8));
	outb(0x1F5, (uint8_t)(lba >> 16));

	outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));

	outb(0x1F7, 0x20);

	for (int i = 0; i < 4; i++) {
        inb(0x1F7 + 7);
    }

	// 5. Читаем 512 байт (256 слов по 2 байта) из порта данных 0x1F0
    uint16_t *ptr = (uint16_t *) target_buffer;
    for (int i = 0; i < 256; i++) {
        ptr[i] = inw(0x1F0); // inw - чтение 16 бит из порта
    }

	return 0;
}
