#include "idt.h"

struct idt_entry idt[256];
struct idt_ptr ptr;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
	idt[num].base_low = base & 0xFFFF;
	idt[num].base_high = (base >> 16) & 0xFFFF;
	idt[num].always0 = 0;
	idt[num].flags = flags;
	idt[num].sel = sel;
}
