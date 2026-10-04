#include <stdint.h>
#include "font8x16.h" // Сам массив font8x16[128][16]


#define SCREEN_WIDTH  1024

/* =========================================================================
 * 2. Цветовая палитра CatOS (Формат 0x00RRGGBB / ARGB32)
 * ========================================================================= */
#define COLOR_DARK_BG      0x001A1A24  /* Глубокий темный фиолетово-синий */
#define COLOR_PASTEL_PINK  0x00FFB6C1  /* Нежно-розовый */
#define COLOR_HOT_PINK     0x00FF69B4  /* Ярко-розовый */
#define COLOR_WHITE        0x00FFFFFF  /* Белый */
#define COLOR_BLACK        0x00000000  /* Черный */
#define COLOR_PURPLE       0x008A2BE2  /* Фиолетовый акцент */

#define SCREEN_HEIGHT 768

static uint32_t* const FRAMEBUFFER = (uint32_t*)0xFD000000;

void put_pixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
    FRAMEBUFFER[y * SCREEN_WIDTH + x] = color;
}

void gfx_clear(uint32_t color) {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        FRAMEBUFFER[i] = color;
    }
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            put_pixel(x + j, y + i, color);
        }
    }
}


/**
 * Отрисовка одного символа с масштабированием
 * @param scale Множитель размера (1 = 8x16, 2 = 16x32, 3 = 24x48)
 */
void draw_char(int x, int y, char c, uint32_t fg_color, uint32_t bg_color, int scale) {
    if ((unsigned char)c >= 128) return;
    if (scale < 1) scale = 1;

    // Берем 16 байт матрицы шрифта для символа c
    const uint8_t* glyph = font8x16[(unsigned char)c];

    for (int cy = 0; cy < 16; cy++) {
        uint8_t row = glyph[cy];
        for (int cx = 0; cx < 8; cx++) {
            // Проверяем бит слева направо (от 7 до 0)
            int is_set = row & (1 << (7 - cx));

            if (is_set) {
                // Если бит 1 — рисуем пиксель/квадратик цвета текста
                draw_rect(x + cx * scale, y + cy * scale, scale, scale, fg_color);
            } else if (bg_color != 0) {
                // Если бит 0 и фон не прозрачный — рисуем фон
                draw_rect(x + cx * scale, y + cy * scale, scale, scale, bg_color);
            }
        }
    }
}

/**
 * Отрисовка строки текста
 */
void draw_string(int x, int y, const char* str, uint32_t fg_color, uint32_t bg_color, int scale) {
    int cur_x = x;
    while (*str) {
        if (*str == '\n') {
            y += 16 * scale; // Переход на новую строку
            cur_x = x;
        } else {
            draw_char(cur_x, y, *str, fg_color, bg_color, scale);
            cur_x += 8 * scale; // Шаг вправо на ширину символа
        }
        str++;
    }
}

// Отрисовка фирменного логотипа CatOS
void draw_catos_logo(int center_x, int center_y) {
    uint32_t pink_color = 0x00FF69B4;  // Розовый
    uint32_t white_color = 0x00FFFFFF; // Белый

    // 1. Кошачьи ушки (левое и правое)
    // Левое ушко
    for (int i = 0; i < 30; i++) {
        draw_rect(center_x - 50 + i, center_y - 80 + i, 30 - i, 1, pink_color);
    }
    // Правое ушко
    for (int i = 0; i < 30; i++) {
        draw_rect(center_x + 20, center_y - 80 + i, 30 - i, 1, pink_color);
    }

    // 2. Голова котика (основание)
    draw_rect(center_x - 50, center_y - 50, 100, 60, pink_color);

    // 3. Глазки (белые пиксели)
    draw_rect(center_x - 30, center_y - 35, 12, 12, white_color);
    draw_rect(center_x + 18, center_y - 35, 12, 12, white_color);

    // 4. Подушечка лапки снизу логотипа
    draw_rect(center_x - 25, center_y + 25, 50, 35, pink_color);
    draw_rect(center_x - 35, center_y + 15, 12, 15, pink_color);
    draw_rect(center_x - 15, center_y + 5,  12, 15, pink_color);
    draw_rect(center_x + 3,  center_y + 5,  12, 15, pink_color);
    draw_rect(center_x + 23, center_y + 15, 12, 15, pink_color);

	draw_string(300, 100, "CatOS", COLOR_PASTEL_PINK, 0, 3);
	draw_string(300, 600, "By Poyarik", COLOR_PASTEL_PINK, 0, 1);
}
