#include "mem.h"

int main(int argc, char const *argv[])
{
    mem_heap_t* heap = mem_heap_create(0, __FILE__, __LINE__);

    char *str = mem_heap_alloc(heap, 10);
    memcpy(str, "hellp", 5);

    char *str2 = mem_heap_printf(heap, "str: %s, num: %d", str, 123);

    printf("%s\n", str);

    mem_heap_free(heap);
    
    return 0;
}
