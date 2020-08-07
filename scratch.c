
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <table/table.h>

int main() {
    char* token;
    char* request = "set\tname\tfoo";
    char* copy = (char*)malloc(strlen(request) + 1);
    
    strcpy(copy, request);
    token = strtok(copy, "\t");
    puts(token);

    token = strtok(NULL, "\t");
    puts(token);

    token = strtok(NULL, "\t");
    puts(token);

    free(copy);
    copy = NULL;
}
