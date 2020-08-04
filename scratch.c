
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    char* token;
    char* request = "get\tname";
    char* copy = (char*)malloc(strlen(request) + 1);
    
    strcpy(copy, request);
    token = strtok(copy, "\t");
    puts(token);

    while ((token = strtok(NULL, "\t"))) {
        puts(token);
    }

    free(copy);
    copy = NULL;
}
