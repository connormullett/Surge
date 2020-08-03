
#include "table.h"
#include <stdio.h>

uint hash(const char* key) {
    uint hash_index = 0;
    uint i = 0;
    int key_len = strlen(key);

    for (; i < key_len; i++)
        hash_index = hash_index * 37 + key[i];
    hash_index = hash_index % TABLE_SIZE;

    return hash_index;
}

table_t* table_t_init(void) {
    table_t* table = (table_t*)malloc(sizeof(table_t));
    table->entries = (entry**)malloc(sizeof(entry*) * TABLE_SIZE);
    table->capacity = TABLE_SIZE;
    table->size = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        table->entries[i] = NULL;
    }

    return table;
}

entry* create_entry(char* key, char* value) {
    entry* e = (entry*)malloc(sizeof(entry));
    e->key = key;
    e->value = value;
    e->next = NULL;
    return e;
}


void table_t_set(table_t* t, char* key, char* value) {
    entry* e = create_entry(key, value);

    uint h = hash(key);

    entry* tmp = t->entries[h];

    if (tmp == NULL) {
        t->entries[h] = e;
        t->size++;
        return;
    }

    // loop list
    while ( tmp != NULL ) {
        
        // update value if key exists
        if ( strcmp(key, tmp->key) == 0 ) {
            tmp->value = value;
            t->size++;
            free(e);
            return;
        }

        // at end of list, add new entry
        if (tmp->next == NULL) {
            tmp->next = e;
            t->size++;
            return;
        }

        // continue
        tmp = tmp->next;
    }

    // tmp should be NULL
    tmp->next = e;
    t->size++;

    // check if resize needed
    if (t->size > table_t_get_load_factor(t)) {
        table_t_resize_and_map(t);
    }
}

char* table_t_get(table_t* t, const char* key) {
    uint h = hash(key);

    entry* e = t->entries[h];

    while (e != NULL) {
        if ( e->key != NULL && strcmp(key, e->key) == 0 ) {
            return e->value;
        }

        e = e->next;
    }

    return NULL;
}


void table_t_resize_and_map(table_t* t) {
    printf("Not implemented\n");
}


void table_t_delete(table_t* t, const char* key) {
    uint h = hash(key);

    // nothing at index
    if (t->entries[h] == NULL) {
        return;
    }

    entry* tmp;
    entry* prev = NULL;
    entry* entry = t->entries[h];

    while (entry != NULL) {
        if ( strcmp(entry->key, key) == 0 ) {
            // first of list
            if (prev == NULL) {
                tmp = entry;
                t->entries[h] = entry->next;
                tmp->key = NULL;
                tmp->value = NULL;
                return;
            } else {
                if (entry->next == NULL) {
                    // if next is null, end of list
                    entry->key = NULL;
                    entry->value = NULL;
                    prev->next = NULL;
                    return;
                } else {
                    prev->next = prev->next->next;
                    entry->key = NULL;
                    entry->value = NULL;
                    entry->next = NULL;
                }
            }
        }

        prev = entry;
        entry = entry->next;
    }

}


void table_t_dump(table_t* t) {
    entry** entries = t->entries;

    for (int i = 0; i < TABLE_SIZE; i++) {
        entry* e = entries[i];

        if (e == NULL) continue;

        while(e != NULL) {
            printf("index [%d]: '%s', '%s' -> ", i, e->key, e->value);
            e = e->next;
        }

        printf("\n");
    }
}


uint table_t_get_capacity(table_t* t) {
    return t->capacity;
}


uint table_t_get_size(table_t* t) {
    return t->size;
}

int table_t_get_load_factor(table_t* t) {
    float one_half = (float)3 / 2;
    return (int)(float)t->capacity / one_half;  
}
