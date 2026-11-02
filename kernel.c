#include "keyboard.h"
#include "shell.h"
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

	if (read_sector(1, boot_sector) == 0) {
		uint16_t signature = *((uint16_t *)(boot_sector + 510));
		
		if (signature == 0xAA55) {
			create_window("Drive working", 600, 300);

			fat32_bpb_t *bpb = (fat32_bpb_t *) boot_sector;

			int data_start = bpb->reserved_sectors + (bpb->num_fats * bpb->fat_size_32);
			uint8_t data[512];

			read_sector(data_start, data);

			
		}
	}


	while (1) {
		__asm__ __volatile__("hlt");
	}
	//println(prompt);
	//
	// run_shell();
}
