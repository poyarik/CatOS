#include "timer.h"
#include "io.h"
#include <stdint.h>

volatile uint64_t ticks = 0;

void timer_handler_c(void) {
	ticks++;
	outb(0x20, 0x20);
}

void delay(float time) {
	ticks = 0;

	while (ticks < time * 100) {
		__asm__ __volatile__("hlt");
	}
}
