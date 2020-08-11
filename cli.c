
#include "cli.h"
#include "thread_pool.h"
#include "table.h"

#include <string.h>
#include <stdbool.h>

void* command_line(void* arg) {
    table_t* table = (table_t*)arg;
    int input;
    bool error_flag = false;
    
    char buffer[MAX_COMMAND_SIZE];
    const char* prompt = "> ";
    int sz = sizeof(buffer);

    while (1) {
        // zero buffer
        memset(buffer, 0, MAX_COMMAND_SIZE);

        input = get_line(prompt, buffer, sz);

        // error checking
        if (input == NO_INPUT) {
            puts("no input"); error_flag = true;
        }

        if (input == TOO_LONG) {
            puts("too long"); error_flag = true;
        }

        pthread_mutex_lock(&table_thread_lock);
        if (strcmp(buffer, "dump") == 0) {
            if (table->size == 0) puts("NONE");
            else table_t_dump(table);
        }
        pthread_mutex_unlock(&table_thread_lock);

    } // while
}


int get_line(const char* prompt, char* buff, size_t sz) {
    int ch, extra;

    // get line with buffer overflow protection
    if (prompt != NULL) {
        printf("%s ", prompt);
        fflush(stdout);
    }

    if (fgets (buff, sz, stdin) == NULL) return NO_INPUT;

    if (buff[strlen(buff) - 1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    buff[strlen(buff) - 1] = '\0';
    return OK;
}

