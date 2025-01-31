/**
 * @file heap.c
 * @date 2025-01-28
 * @author yuesong-feng
 */
#include "heap.h"
#include "calc.h"
#include <assert.h>
#include <stdlib.h>

#define MEM_ALIGNMENT 8
#define MEM_BLOCK_HEADER_SIZE calc_align(sizeof(mem_block_t), MEM_ALIGNMENT)
#define MEM_SPACE_NEEDED(N) calc_align((N), MEM_ALIGNMENT)
#define MEM_BLOCK_START_SIZE 64
#define MEM_BLOCK_STANDARD_SIZE 8000

static mem_block_t *mem_heap_create_block(mem_heap_t *heap, size_t n) {
  mem_block_t *block;
  size_t len;

  len = MEM_BLOCK_HEADER_SIZE + MEM_SPACE_NEEDED(n);

  block = malloc(len);
  assert(block);

  block->len = len;
  block->free = MEM_BLOCK_HEADER_SIZE;
  block->start = MEM_BLOCK_HEADER_SIZE;

  if (heap == NULL) {
    block->total_size = len;
  } else {
    heap->total_size += len;
  }

  assert(MEM_BLOCK_HEADER_SIZE < len);

  return block;
}

mem_heap_t *mem_heap_create(size_t n) {
  mem_block_t *block;

  if (!n) {
    n = MEM_BLOCK_START_SIZE;
  }

  block = mem_heap_create_block(NULL, n);
  assert(block);

  LIST_INIT(block->base);

  LIST_ADD_FIRST(list, block->base, block);

  return block;
}

static void mem_heap_block_free(mem_heap_t *heap, mem_block_t *block) {
  LIST_REMOVE(list, block->base, block);

  assert(heap->total_size >= block->len);
  heap->total_size -= block->len;

  free(block);
}

void mem_heap_free(mem_heap_t *heap) {
  mem_block_t *block;
  mem_block_t *prev_block;

  block = LIST_GET_LAST(heap->base);
  while (block != NULL) {
    prev_block = LIST_GET_PREV(list, block);

    mem_heap_block_free(heap, block);

    block = prev_block;
  }
}

static mem_block_t *mem_heap_add_block(mem_heap_t *heap, size_t n) {
  mem_block_t *block;
  mem_block_t *new_block;
  size_t new_size;

  block = LIST_GET_LAST(heap->base);

  new_size = 2 * block->len;

  if (new_size > MEM_BLOCK_STANDARD_SIZE) {
    new_size = MEM_BLOCK_STANDARD_SIZE;
  }

  if (new_size < n) {
    new_size = n;
  }

  new_block = mem_heap_create_block(heap, new_size);
  assert(new_block);

  LIST_INSERT_AFTER(list, heap->base, block, new_block);

  return block;
}

void *mem_heap_alloc(mem_heap_t *heap, size_t n) {
  mem_block_t *block;
  void *buf;

  block = LIST_GET_LAST(heap->base);

  if (block->len < block->free + MEM_SPACE_NEEDED(n)) {
    block = mem_heap_add_block(heap, n);
    assert(block);
  }

  buf = (byte *)block + block->free;

  block->free += MEM_SPACE_NEEDED(n);

  return buf;
}

byte *mem_heap_get_heap_top(mem_heap_t *heap) {
  mem_block_t *block;
  byte *buf;

  block = LIST_GET_LAST(heap->base);

  buf = (byte *)block + block->free;

  return buf;
}

void mem_heap_free_heap_top(mem_heap_t *heap, byte *old_top) {
  mem_block_t *block;
  mem_block_t *prev_block;

  block = LIST_GET_LAST(heap->base);
  while (block != NULL) {
    if (((byte *)block + block->free >= old_top) && ((byte *)block <= old_top)) {
      break;
    }

    prev_block = LIST_GET_PREV(list, block);

    mem_heap_block_free(heap, block);

    block = prev_block;
  }

  assert(block);

  block->free = old_top - (byte *)block;

  assert(block->start <= block->free);

  if ((heap != block) && block->free == block->start) {
    mem_heap_block_free(heap, block);
  }
}

void mem_heap_empty(mem_heap_t *heap) {
  mem_heap_free_heap_top(heap, (byte *)heap + heap->start);
}

void *mem_heap_get_top(mem_heap_t *heap, size_t n) {
  mem_block_t *block;
  void *buf;

  block = LIST_GET_LAST(heap->base);

  buf = (byte *)block + block->free - MEM_SPACE_NEEDED(n);

  return buf;
}

void mem_heap_free_top(mem_heap_t *heap, size_t n) {
  mem_block_t *block;

  block = LIST_GET_LAST(heap->base);

  block->free -= MEM_SPACE_NEEDED(n);
  assert(block->start <= block->free);

  if ((heap != block) && (block->free == block->start)) {
    mem_heap_block_free(heap, block);
  }
}

size_t mem_heap_get_size(mem_heap_t *heap) {
  return heap->total_size;
}