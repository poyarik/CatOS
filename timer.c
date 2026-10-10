#include "timer.h"
#include "io.h"
#include "framebuffer.h"

int ticks = 0;

void timer_handler_c(void) {
	ticks++;
	draw_char(1, 1, ' ', 0xFFFFFF, 0x000001, 1);
	draw_char(1, 1, (char)ticks, 0xFFFFFF, 0x000000, 1);
	outb(0x20, 0x20);
}
