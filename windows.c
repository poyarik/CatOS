#include "windows.h"
#include "framebuffer.h"

#define TITLE_HEIGHT 20

Window windows[10];
int windows_open = 0;

void draw_window(Window* window) {
	int full_size_y = window->size_y + TITLE_HEIGHT;
	int btn_size = (TITLE_HEIGHT * 3) / 4;

	draw_rect(window->x, window->y, window->size_x, full_size_y, 0x7d5ebf);
	draw_string(window->x + 5, window->y + 5, window->title, 0xFFFFFF, 0, 1);
	draw_rect(window->x, window->y + TITLE_HEIGHT, window->size_x, window->size_y, 0xd1cfd4);
	draw_rect_outline(window->x + window->size_x - btn_size - btn_size / 4, window->y + btn_size / 4, btn_size, btn_size, 1, 0x888888);
	draw_char(window->x + window->size_x - btn_size - btn_size / 4, window->y + btn_size / 4, 'x', 0xFFFFFF, 0x0, 1);
}

void create_window(char* title, int size_x, int size_y) {
	Window window;
	window.title = title;
	window.size_x = size_x;
	window.size_y = size_y;

	window.x = 100;
	window.y = 100;

	windows[windows_open] = window;
	draw_window(&window);

	windows_open++;
}
