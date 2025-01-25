#include "heap.h"
#include "byte.h"
#include "calc.h"
#include <assert.h>
#include <stdlib.h>

#define MEM_ALIGNMENT 8
#define MEM_BLOCK_HEADER_SIZE calc_align(sizeof(mem_block_t), MEM_ALIGNMENT)
#define MEM_SPACE_NEEDED(size) calc_align(size, MEM_ALIGNMENT)

static mem_block_t *mem_heap_create_block(mem_heap_t *heap, size_t size) {
  mem_block_t *block;
  size_t len;

  len = MEM_BLOCK_HEADER_SIZE + MEM_SPACE_NEEDED(size);

  block = malloc(len);
  assert(block != NULL);

  block->len = len;
  block->free = MEM_BLOCK_HEADER_SIZE;
  block->start = MEM_BLOCK_HEADER_SIZE;

  if (heap == NULL)
    block->total_size = len;
  else
    block->total_size += len;

  return block;
}

#define MEM_BLOCK_START_SIZE 64

mem_heap_t *mem_heap_create(size_t size) {
  mem_block_t *block;

  if (!size)
    size = MEM_BLOCK_START_SIZE;

  block = mem_heap_create_block(NULL, size);
  assert(block != NULL);

  LIST_INIT(block->base);

  LIST_ADD_LAST(list, block->base, block);

  return block;
}

static void mem_heap_block_free(mem_heap_t *heap, mem_block_t *block) {
  LIST_REMOVE(list, heap->base, block);

  heap->total_size -= block->len;

  free(block);
}

void mem_heap_free(mem_heap_t *heap) {
  mem_block_t *block, *prev_block;

  block = LIST_GET_LAST(heap->base);

  while (block != NULL) {
    prev_block = LIST_GET_PREV(list, block);

    mem_heap_block_free(heap, block);

    block = prev_block;
  }
}

#define MEM_BLOCK_STANDARD_SIZE 8000

static mem_block_t *mem_heap_add_block(mem_heap_t *heap, size_t size) {
  mem_block_t *block, *new_block;
  size_t new_size;

  block = LIST_GET_LAST(heap->base);

  new_size = 2 * block->len;

  if (new_size > MEM_BLOCK_STANDARD_SIZE)
    new_size = MEM_BLOCK_STANDARD_SIZE;

  if (new_size < size)
    new_size = size;

  new_block = mem_heap_create_block(heap, new_size);
  assert(new_block != NULL);

  LIST_INSERT_AFTER(list, heap->base, block, new_block);

  return new_block;
}

void *mem_heap_alloc(mem_heap_t *heap, size_t size) {
  mem_block_t *block;
  void *buf;

  block = LIST_GET_LAST(heap->base);

  if (block->len < block->free + MEM_SPACE_NEEDED(size)) {
    block = mem_heap_add_block(heap, size);
    assert(block != NULL);
  }

  buf = (byte *)block + block->free;

  block->free += MEM_SPACE_NEEDED(size);

  return buf;
}