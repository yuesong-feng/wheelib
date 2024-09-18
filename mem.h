/*

  mem.h

  memory utilities, from MySQL 8.4.0 InnoBase
  alloc memory fail will cause system halt immediately,
  so it's unnecessary for upper invocation to consider NULL-return

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05

*/
#ifndef MEM_H
#define MEM_H
#include "lst.h"  // for list
#include "byte.h" // for byte
#include "calc.h" // for calc_align
#include <string.h> // for memcpy

typedef struct mem_block_t mem_block_t;
struct mem_block_t {
  LIST(mem_block_t) base;
  LIST_NODE(mem_block_t) link;
  size_t len;
  size_t total_size;
  size_t free;
  size_t start;
};

typedef mem_block_t mem_heap_t;

#define MEM_BLOCK_START_SIZE 64
#define MEM_BLOCK_STANDARD_SIZE 8000
#define MEM_ALIGNMENT 8
#define MEM_BLOCK_HEADER_SIZE calc_align(sizeof(mem_block_t), MEM_ALIGNMENT)

static inline uint64_t MEM_SPACE_NEEDED(uint64_t N) {
  return calc_align(N, MEM_ALIGNMENT);
}

static inline mem_block_t *mem_heap_create_block(mem_heap_t *heap, size_t n) {
  mem_block_t *block;
  size_t len;

  len = MEM_BLOCK_HEADER_SIZE + MEM_SPACE_NEEDED(n);

  block = malloc(len);
  halt_if(block == NULL);

  block->len = len;
  block->start = MEM_BLOCK_HEADER_SIZE;
  block->free = MEM_BLOCK_HEADER_SIZE;

  if (heap == NULL)
    block->total_size = len;
  else
    heap->total_size += len;

  return block;
}

static inline mem_heap_t *mem_heap_create(size_t size) {
  mem_block_t *block;

  if (!size)
    size = MEM_BLOCK_START_SIZE;

  block = mem_heap_create_block(NULL, size);

  LIST_INIT(block->base);

  LIST_ADD_FIRST(link, block->base, block);

  return block;
}

static inline void mem_heap_block_free(mem_heap_t *heap, mem_block_t *block) {
  LIST_REMOVE(link, heap->base, block);

  heap->total_size -= block->len;

  free(block);
}

static inline void mem_heap_free(mem_heap_t *heap) {
  mem_block_t *block;
  mem_block_t *prev_block;

  block = LIST_GET_LAST(heap->base);

  while (block != NULL) {
    prev_block = LIST_GET_PREV(link, block);

    mem_heap_block_free(heap, block);

    block = prev_block;
  }
}

static inline mem_block_t *mem_heap_add_block(mem_heap_t *heap, size_t n) {
  mem_block_t *block;
  mem_block_t *new_block;
  size_t new_size;

  block = LIST_GET_LAST(heap->base);

  new_size = 2 * block->len;

  if (new_size > MEM_BLOCK_STANDARD_SIZE)
    new_size = MEM_BLOCK_STANDARD_SIZE;

  if (new_size < n)
    new_size = n;

  new_block = mem_heap_create_block(heap, new_size);

  LIST_INSERT_AFTER(link, heap->base, block, new_block);

  return new_block;
}

static inline void *mem_heap_alloc(mem_heap_t *heap, size_t n) {
  mem_block_t *block;
  void *buf;
  size_t free;

  block = LIST_GET_LAST(heap->base);

  if (block->len < block->free + MEM_SPACE_NEEDED(n)) {
    block = mem_heap_add_block(heap, n);
  }

  free = block->free;

  buf = (byte *)block + free;

  block->free = free + MEM_SPACE_NEEDED(n);

  return buf;
}

static inline void *mem_heap_zalloc(mem_heap_t *heap, size_t n) {
  return memset(mem_heap_alloc(heap, n), 0, n);
}

static inline byte *mem_heap_get_heap_top(mem_heap_t *heap) {
  mem_block_t *block;
  byte *buf;

  block = LIST_GET_LAST(heap->base);

  buf = (byte *)block + block->free;

  return buf;
}

static inline void mem_heap_free_heap_top(mem_heap_t *heap, byte *old_top) {
  mem_block_t *block;
  mem_block_t *prev_block;

  block = LIST_GET_LAST(heap->base);

  while (block != NULL) {
    if (((byte *)block + block->free >= old_top) && ((byte *)block <= old_top))
      break;

    prev_block = LIST_GET_PREV(link, block);

    mem_heap_block_free(heap, block);

    block = prev_block;
  }

  if (old_top == (byte *)block + block->free)
    return;

  block->free = old_top - (byte *)block;

  if ((heap != block) && (block->free == block->start)) {
    mem_heap_block_free(heap, block);
  }
}

static inline void mem_heap_empty(mem_heap_t *heap) {
  mem_heap_free_heap_top(heap, (byte *)heap + heap->start);
}

// TODO: n is bigger than block size?
static inline void *mem_heap_get_top(mem_heap_t *heap, size_t n) {
  mem_block_t *block;
  byte *buf;

  block = LIST_GET_LAST(heap->base);

  buf = (byte *)block + block->free - MEM_SPACE_NEEDED(n);

  return (void *)buf;
}

// TODO: n is bigger than block size?
static inline void mem_heap_free_top(mem_heap_t *heap, size_t n) {
  mem_block_t *block;

  block = LIST_GET_LAST(heap->base);

  block->free = block->free - MEM_SPACE_NEEDED(n);

  if ((heap != block) && (block->free == block->start)) {
    mem_heap_block_free(heap, block);
  }
}

static inline void *mem_heap_dup(mem_heap_t *heap, const void *data,
                                 size_t len) {
  return memcpy(mem_heap_alloc(heap, len), data, len);
}

static inline char *mem_heap_strdup(mem_heap_t *heap, const char *str) {
  return (char *)mem_heap_dup(heap, str, strlen(str) + 1);
}

static inline char *mem_heap_strdupl(mem_heap_t *heap, const char *str,
                                     size_t len) {
  char *s = (char *)mem_heap_alloc(heap, len + 1);
  s[len] = 0;
  if (len > 0)
    memcpy(s, str, len);
  return s;
}

static inline char *mem_heap_strcat(mem_heap_t *heap, const char *s1,
                                    const char *s2) {
  char *s;
  size_t s1_len = strlen(s1);
  size_t s2_len = strlen(s2);

  s = (char *)mem_heap_alloc(heap, s1_len + s2_len + 1);

  memcpy(s, s1, s1_len);
  memcpy(s + s1_len, s2, s2_len);

  s[s1_len + s2_len] = '\0';

  return s;
}

static inline void *mem_alloc(size_t n) {
  mem_heap_t *heap;
  void *buf;

  heap = mem_heap_create(n);

  buf = mem_heap_alloc(heap, n);

  return buf;
}

static inline void mem_free(void *ptr) {
  mem_heap_t *heap;

  heap = (mem_heap_t *)((byte *)ptr - MEM_BLOCK_HEADER_SIZE);

  mem_heap_free(heap);
}

#endif