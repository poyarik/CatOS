#ifndef WINDOWS_H
#define WINDOWS_H

typedef struct {
	int x;
	int y;
	char* title;
	int size_x;
	int size_y;
} Window;

void create_window(char* title, int size_x, int size_y);

#endif
