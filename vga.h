#ifndef VGA_H
#define VGA_H

#define WIDTH 80
#define HEIGHT 25
#define DEF_CLR 0x1F

// Функции работы со строками и задержкой
int strlen(const char* str);
void delay(volatile unsigned long int count);

// VGA-функции
unsigned short convert_to_vga(char ch, unsigned char clr);
void clear_vga(void);
void type_vga(int row, int cursor, char ch);
void print_pr(int row, int base_cur, const char* prompt);

#endif
