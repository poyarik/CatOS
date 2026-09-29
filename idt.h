#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// Структура одной записи IDT (Gate Descriptor)
struct idt_entry {
    uint16_t base_low;   // Младшие 16 бит адреса функции-обработчика
    uint16_t sel;        // Селектор сегмента кода (для ядра обычно 0x08)
    uint8_t  always0;    // Всегда 0
    uint8_t  flags;      // Флаги доступа и тип гейта (0x8E - 32-bit Interrupt Gate)
    uint16_t base_high;  // Старшие 16 бит адреса функции-обработчика
} __attribute__((packed));

// Указатель на таблицу IDT для инструкции 'lidt'
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void remap_pic(void);
void idt_init(void);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

#endif
