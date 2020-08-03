
#ifndef _TABLE_H_
#define _TABLE_H_

#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 2
#define MAX_TABLE_SIZE 4294967295  // uint max
#define MAX_LOAD_FACTOR 1.5

typedef unsigned int uint;

typedef struct entry {
    char* key;
    char* value;
    struct entry* next;
} entry;

typedef struct table_t {
    uint capacity;
    uint size;
    entry** entries;
} table_t;

void table_t_dump(table_t* t);
uint hash(const char* key);
table_t* table_t_init(void);
char* table_t_get(table_t* t, const char* key);
void table_t_set(table_t* t, char* key, char* value);

uint table_t_get_capacity(table_t* t);
uint table_t_get_size(table_t* t);
int table_t_get_load_factor(table_t* t);
void table_t_delete(table_t* t, const char* key);

void table_t_resize_and_map(table_t* t);

#endif
