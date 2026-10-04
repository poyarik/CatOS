#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

/* =========================================================================
 * 1. Структура Multiboot Info (получаем от GRUB / QEMU через регистр EBX)
 * ========================================================================= */
typedef struct {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;

    /* Поля линейного фреймбуфера (VBE / Graphics) */
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;
    uint8_t  framebuffer_type;
    uint8_t  color_info[6];
} __attribute__((packed)) multiboot_info_t;

/* =========================================================================
 * 3. Прототипы функций графического движка
 * ========================================================================= */

/**
 * Инициализация фреймбуфера из данных Multiboot.
 * @param mb_info Указатель на структуру multiboot_info_t, переданную в kmain
 */
void framebuffer_init(multiboot_info_t* mb_info);

/**
 * Установка цвета конкретного пикселя.
 * @param x Координата X (0 .. width - 1)
 * @param y Координата Y (0 .. height - 1)
 * @param color Цвет в формате 0x00RRGGBB
 */
void put_pixel(int x, int y, uint32_t color);

/**
 * Полная закраска всего экрана выбранным цветом.
 * @param color Цвет в формате 0x00RRGGBB
 */
void gfx_clear(uint32_t color);

/**
 * Отрисовка закрашенного прямоугольника.
 * @param x Начальная координата X
 * @param y Начальная координата Y
 * @param width Ширина прямоугольника
 * @param height Высота прямоугольника
 * @param color Цвет заливки
 */
void draw_rect(int x, int y, int width, int height, uint32_t color);

/**
 * Отрисовка контурного (незакрашенного) прямоугольника.
 */
void draw_rect_outline(int x, int y, int width, int height, uint32_t color);

/**
 * Отрисовка фирменного логотипа CatOS (кошачья лапка / ушки).
 * @param center_x Центр логотипа по X
 * @param center_y Центр логотипа по Y
 */
void draw_catos_logo(int center_x, int center_y);

/**
 * Отрисовка одного ASCII символа по координатам пикселей (для шрифта).
 * @param x Координата X
 * @param y Координата Y
 * @param c Символ ASCII
 * @param fg_color Цвет текста
 * @param bg_color Цвет фона (или 0 для прозрачности)
 */
void draw_char(int x, int y, char c, uint32_t fg_color, uint32_t bg_color, int scale);

void draw_string(int x, int y, const char* str, uint32_t fg_color, uint32_t bg_color, int scale);

#endif
