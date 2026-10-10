#include "idt.h"
#include "io.h"
#include <stdint.h>

struct idt_entry idt[256];
struct idt_ptr ptr;

extern void asm_idt_load(uint32_t idt_ptr_address);
extern void asm_keyboard_addr(void);
extern void asm_timer_addr(void);

void remap_pic(void) {
	outb(0x20, 0x11);
    outb(0xA0, 0x11);

	// Ремапим
	outb(0x21, 0x20);
	outb(0xA1, 0x28);

	outb(0x21, 0x04);
	outb(0xA1, 0x02);

	// Обратно в режим архитектуры
	outb(0x21, 0x01);
	outb(0xA1, 0x01);

	outb(0x21, 0x00);
	outb(0xA1, 0x00);
	
	outb(0x21, 0xFC); // 0xFD = 1111 1100b (IRQ0, IRQ1)
	outb(0xA1, 0xFF); // Заблокировать всё на Slave PIC
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
	idt[num].base_low = base & 0xFFFF;
	idt[num].base_high = (base >> 16) & 0xFFFF;
	idt[num].always0 = 0;
	idt[num].flags = flags;
	idt[num].sel = sel;
}

void idt_init(void) {
	ptr.limit = sizeof(struct idt_entry) * 256 - 1;
	ptr.base = (uint32_t) &idt;

	struct idt_entry null_idt;
	null_idt.base_low = 0;
	null_idt.base_high = 0;
	null_idt.always0 = 0;
	null_idt.flags = 0;
	null_idt.sel = 0;

	// Затираю нулями
	for (int i = 0; i < 256; i++) {
		idt[i] = null_idt;
	}

	remap_pic();

	idt_set_gate(32, (uint32_t)asm_timer_addr, 0x10, 0x8E);
	idt_set_gate(33, (uint32_t)asm_keyboard_addr, 0x10, 0x8E);

	asm_idt_load((uint32_t)&ptr);

	// задаю частоту таймера
	uint32_t divisor = 1193182 / 100; // 100 Гц = каждые 10 мс
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));

	__asm__ __volatile__("sti");
} 
