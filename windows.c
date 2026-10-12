#include "windows.h"
#include "framebuffer.h"

Window windows[10];
int windows_open = 0;

void draw_window(Window* window) {
	int full_size_y = window->size_y + 50;

	draw_rect(window->x, window->y, window->size_x, full_size_y, 0x7d5ebf);
	draw_string(window->x + 10, window->y + 10, window->title, 0xFFFFFF, 0, 1);
	draw_rect(window->x, window->y + 50, window->size_x, window->size_y, 0xd1cfd4);
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
