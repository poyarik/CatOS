#include "keyboard.h"
#include "vga.h"
#include "shell.h"
#include "idt.h"
#include "io.h"
#include "framebuffer.h"
#include <stdint.h>

uint32_t* framebuffer = 0;

extern char* prompt;
extern volatile unsigned short* vga;
extern int row;
extern int cursor;

void kmain(multiboot_info_t* mb_info) {
	framebuffer = (uint32_t*)(uint32_t)mb_info->framebuffer_addr;

	gfx_clear(0x001A1A24);

	draw_catos_logo(512, 384);

	while (1) {
		__asm__ __volatile__("hlt");
	}

	delay(4000);

	gfx_clear(0x001A1A24);
	idt_init();
	println(prompt);
	//
	// run_shell();
}
