/**
 * @file dyn.c
 * @date 2025-01-29
 * @author yuesong-feng
 */
#include "dyn.h"
#include <assert.h>
#include <string.h>

#define DYN_BLOCK_FULL_FLAG 0x1000000UL

dyn_array_t *dyn_array_create(dyn_array_t *arr) {
  assert(arr);
  arr->heap = NULL;
  arr->used = 0;
  return arr;
}

void dyn_array_free(dyn_array_t *arr) {
  if (arr->heap != NULL)
    mem_heap_free(arr->heap);
}

static dyn_block_t *dyn_array_add_block(dyn_array_t *arr) {
  mem_heap_t *heap;
  dyn_block_t *block;

  assert(arr);

  if (arr->heap != NULL) {
    LIST_INIT(arr->base);
    LIST_ADD_FIRST(list, arr->base, arr);
    arr->heap = mem_heap_create(sizeof(dyn_block_t));
  }

  block = dyn_array_get_last_block(arr);
  block->used = block->used | DYN_BLOCK_FULL_FLAG;

  heap = arr->heap;

  block = (dyn_block_t *)mem_heap_alloc(heap, sizeof(dyn_block_t));
  block->used = 0;

  LIST_ADD_LAST(list, arr->base, block);

  return block;
}

byte *dyn_array_open(dyn_array_t *arr, size_t size) {
  dyn_block_t *block;

  assert(arr);
  assert(size <= DYN_ARRAY_DATA_SIZE);
  assert(size);

  block = arr;

  if (block->used + size > DYN_ARRAY_DATA_SIZE) {
    block = dyn_array_get_last_block(arr);
    if (block->used + size > DYN_ARRAY_DATA_SIZE) {
      block = dyn_array_add_block(arr);
      assert(size <= DYN_ARRAY_DATA_SIZE);
    }
  }

  assert(block->used <= DYN_ARRAY_DATA_SIZE);

  return block->data + block->used;
}

void dyn_array_close(dyn_array_t *arr, const byte *ptr) {
  dyn_block_t *block;

  assert(arr);

  block = dyn_array_get_last_block(arr);

  block->used = ptr - block->data;

  assert(block->used <= DYN_ARRAY_DATA_SIZE);
}

void *dyn_array_push(dyn_array_t *arr, size_t size) {
  dyn_block_t *block;
  size_t used;

  assert(arr);
  assert(size <= DYN_ARRAY_DATA_SIZE);
  assert(size);

  if (block->used + size > DYN_ARRAY_DATA_SIZE) {
    block = dyn_array_get_last_block(arr);
    if (block->used + size > DYN_ARRAY_DATA_SIZE) {
      block = dyn_array_add_block(arr);
    }
  }

  used = block->used;

  block->used = used + size;
  assert(block->used <= DYN_ARRAY_DATA_SIZE);

  return block->data + used;
}

void *dyn_array_get_element(const dyn_array_t *arr, size_t pos) {
  const dyn_block_t *block;

  assert(arr);

  block = dyn_array_get_first_block(arr);

  if (arr->heap != NULL) {
    for (;;) {
      size_t used = dyn_block_get_used(block);
      if (pos < used)
        break;
      pos -= used;
      block = LIST_GET_NEXT(list, block);
      assert(block);
    }
  }

  assert(block);
  assert(dyn_block_get_used(block) >= pos);

  return (byte *)block->data + pos;
}

size_t dyn_array_get_data_size(const dyn_array_t *arr) {
  const dyn_block_t *block;
  size_t sum = 0;

  assert(arr);

  if (arr->heap == NULL)
    return arr->used;

  block = dyn_array_get_first_block(arr);

  while (block != NULL) {
    sum += dyn_block_get_used(block);
    block = dyn_array_get_next_block(arr, block);
  }

  return sum;
}

size_t dyn_block_get_used(const dyn_block_t *block) {
  assert(block);
  return block->used & ~DYN_BLOCK_FULL_FLAG;
}

byte *dyn_block_get_data(dyn_block_t *block) {
  assert(block);
  return (byte *)block->data;
}

void dyn_push_string(dyn_array_t *arr, const char *str, size_t len) {
  size_t n_copied;

  while (len > 0) {
    if (len > DYN_ARRAY_DATA_SIZE) {
      n_copied = DYN_ARRAY_DATA_SIZE;
    } else {
      n_copied = len;
    }

    memcpy(dyn_array_push(arr, n_copied), str, n_copied);

    str += n_copied;
    len -= n_copied;
  }
}