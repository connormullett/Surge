
#ifndef _CLI_H_
#define _CLI_H_

#include <stddef.h>

#define MAX_COMMAND_SIZE 32

void* command_line(void* arg);

#define OK 0
#define NO_INPUT 1
#define TOO_LONG 2
int get_line(const char* prompt, char* buff, size_t sz);

#endif

