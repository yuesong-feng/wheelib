#include "hash.h"
#include <stdio.h>

typedef struct info_t {
    int no;
    char *name;
    hash_node_t hash;
} info_t;

int main(int argc, char const *argv[])
{
    hash_table_t table;
    hash_table_init(&table, 100);

    info_t info1;
    info1.no = 123;
    info1.name = "feng";

    info_t *info2 = malloc(sizeof(info_t));
    info2->no = 234;
    info2->name = "yuesong";

    HASH_INSERT(info_t, hash, &table, 123, &info1);
    HASH_INSERT(info_t, hash, &table, 234, info2);

    info_t *i;
    HASH_SEARCH(hash, &table, 123, info_t *, i, NULL, strcmp(i->name, "feng") == 0);
    if (i == NULL) {
        printf("NULL\n");
    }
    else{
        printf("find info, no: %d, name: %s\n", i->name, i->name);
    }

    HASH_SEARCH(hash, &table, 234, info_t *, i, NULL, strcmp(i->name, "yuesong") == 0);
    if (i == NULL) {
        printf("NULL\n");
    }
    else{
        printf("find info, no: %d, name: %s\n", i->name, i->name);
    }

    hash_table_destroy(&table);
    return 0;
}
