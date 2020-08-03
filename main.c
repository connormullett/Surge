
#include "table.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    table_t* t = table_t_init();

    uint i = hash("key");

    
    table_t_set(t, "key", "value");    
    table_t_set(t, "foo", "bar");
    table_t_set(t, "baz", "biz");
    table_t_set(t, "name", "connor");
    table_t_set(t, "customer", "mike");
    table_t_dump(t);

    printf("delete 'key'\n");
    table_t_delete(t, "key");
    table_t_dump(t); printf("\n");


    printf("set 'key'\n");
    table_t_set(t, "key", "value");
    table_t_dump(t);

    return 0;
}
