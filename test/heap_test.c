#include "heap.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    mem_heap_t *heap = mem_heap_create(1024);

    char *str = mem_heap_alloc(heap, 64);
    memset(str, 'a', 64);

    printf("%s\n", str);

    mem_heap_free(heap);

    return 0;
}
