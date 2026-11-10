#include "keyboard.h"
#include "idt.h"
#include "timer.h"
#include "io.h"
#include "ata.h"
#include "framebuffer.h"
#include "windows.h"
#include <stdint.h>

uint32_t* framebuffer = 0;

extern char* prompt;
extern volatile unsigned short* vga;
extern int row;
extern int cursor;

static inline void debug_out(char c) {
    outb(0xE9, c);
}

void debug_print(const char *str) {
    while (*str) {
        debug_out(*str++);
    }
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

// Превращает байт в строку из двух hex-символов (например, 0xAB -> "AB")
void byte_to_hex(uint8_t byte, char *out_str) {
    char hex_chars[] = "0123456789ABCDEF";
    out_str[0] = hex_chars[(byte >> 4) & 0x0F]; // Старшая тетрада
    out_str[1] = hex_chars[byte & 0x0F];        // Младшая тетрада
    out_str[2] = '\0';
}

void debug_print_hex_byte(uint8_t b) {
    char buf[3];
    byte_to_hex(b, buf);
    debug_out(buf[0]);
    debug_out(buf[1]);

	debug_print("\n");
}

void kmain(multiboot_info_t* mb_info) {
	framebuffer = (uint32_t*)(uint32_t)mb_info->framebuffer_addr;
	
	debug_print("Halllooo\n");

	gfx_clear(0x001A1A24);

	// draw_catos_logo(512, 384);
	idt_init();

	// delay(1);

	// gfx_clear(0x001A1A24);
	
	delay(3);

	uint8_t boot_sector[512];

	for (int try = 0; try < 3; try++){
		if (read_sector(0, boot_sector) == 0) {
			uint16_t signature = *((uint16_t *)(boot_sector + 510));
			
			debug_print("Read: ");
			debug_print_hex_byte((uint8_t)(signature >> 8));
			debug_print_hex_byte((uint8_t)signature);
			debug_print("\n");

			if (signature == 0xAA55) {
				create_window("Drive working", 600, 300);

				fat32_bpb_t *bpb = (fat32_bpb_t *) boot_sector;

				int data_start = bpb->reserved_sectors + (bpb->num_fats * bpb->fat_size_32);
				uint32_t root_dir_lba = data_start + (bpb->root_cluster - 2) * bpb->sectors_per_cluster;

				debug_print("secotr num ");
				debug_print_hex_byte(bpb->sectors_per_cluster);

				for(int s = 0; s < bpb->sectors_per_cluster; s++) {
					debug_print("sector ");
					debug_print_hex_byte(s);
					uint8_t data[512];
					read_sector(root_dir_lba + s, data);

					uint32_t file_cluster = 0;
					uint32_t file_size = 0;

					fat32_direntry_t *entries = (fat32_direntry_t *) data;

					for (int i = 0; i < 16; i++) {
						debug_print("name: ");
						debug_print_hex_byte(entries[i].name[0]);
						debug_print("\n");
						// Проверяем, что это не пустая запись (0x00) и не удаленный файл (0xE5)
						if (entries[i].name[0] == 0x00) continue;
						if (entries[i].name[0] == 0xE5) continue;

						if(strcmp((char *)entries[i].name, "CAT     TXT") == 0) {
							uint32_t file_cluster = ((uint32_t)entries[i].first_cluster_high << 16) | entries[i].first_cluster_low;
							uint32_t file_size = entries[i].file_size;

							uint8_t file_buffer[512];
							uint32_t target_lba = data_start + (file_cluster - 2) * bpb->sectors_per_cluster;

							read_sector(target_lba, file_buffer);

							file_buffer[file_size] = '\0';
							debug_print(file_buffer);
						}
					}
				}
				break;
			}
		}

		debug_print("Not working\n");
		delay(1);
	}


	while (1) {
		__asm__ __volatile__("hlt");
	}
	//println(prompt);
	//
	// run_shell();
}
