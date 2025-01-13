#ifndef MEM_H
#define MEM_H
#include "lst.h"

typedef struct mem_block mem_block_t;
struct mem_block {
    LIST(mem_block_t) base;
    LIST_NODE(mem_block_t) list;
    size_t len;
    size_t total_size;
    size_t free;
    size_t start;
};

typedef mem_block_t mem_heap_t;

mem_heap_t* mem_heap_create(size_t size);
void mem_heap_free(mem_heap_t *heap);
void* mem_heap_alloc(mem_heap_t *heap, size_t size);

#endif