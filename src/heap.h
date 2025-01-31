/**
 * @file heap.h
 * @date 2025-01-28
 * @author yuesong-feng
 */
#ifndef HEAP_H
#define HEAP_H
#include "byte.h"
#include "lst.h"

typedef struct mem_block_t mem_block_t;
struct mem_block_t {
    LIST(mem_block_t) base;
    LIST_NODE(mem_block_t) list;
    size_t len;
    size_t total_size;
    size_t free;
    size_t start;
};

typedef mem_block_t mem_heap_t;

mem_heap_t *mem_heap_create(size_t n);

void mem_heap_free(mem_heap_t *heap);

void *mem_heap_alloc(mem_heap_t *heap, size_t n);

byte *mem_heap_get_heap_top(mem_heap_t *heap);

void mem_heap_free_heap_top(mem_heap_t *heap, byte *old_top);

void mem_heap_empty(mem_heap_t *heap);

void *mem_heap_get_top(mem_heap_t *heap, size_t n);

void mem_heap_free_top(mem_heap_t *heap, size_t n);

size_t mem_heap_get_size(mem_heap_t *heap);

#endif