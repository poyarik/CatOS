#ifndef SHELL_H
#define SHELL_H

#define MAX_CMD_LEN 128

// Сравнение строк
int strcmp(const char* s1, const char* s2);

void shell_execute(const char* cmd_buffer);
void run_shell(void);

#endif
