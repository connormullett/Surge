
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cli.h"


int main() {
    char buffer[100];
    const char* prompt = "> ";
    int sz = sizeof(buffer);

    int rc = get_line(prompt, buffer, sz);

    if (rc == NO_INPUT) {
        puts("no input");
        return 1;
    }

    if (rc == TOO_LONG) {
        puts("too long");
        return 1;
    }

    printf("OK: %s\n", buffer);

    return 0;
}
