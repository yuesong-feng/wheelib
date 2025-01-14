#include "alloc.h"
#include "mem.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

enum {
  alloc_os,
  alloc_heap
};

static void *malloc_fun(alloc_t *alloc, size_t size) {
  switch (alloc->type) {
  case alloc_os:
    assert(alloc->heap == NULL);
    return malloc(size);
  case alloc_heap:
    assert(alloc->heap != NULL);
    return mem_heap_alloc((mem_heap_t *)alloc->heap, size);
  default:
    assert(0);
  }
}

static void free_fun(alloc_t *alloc, void *ptr) {
  switch (alloc->type) {
  case alloc_os:
    assert(alloc->heap == NULL);
    free(ptr);
    break;
  case alloc_heap:
    assert(alloc->heap != NULL);
    break;
  default:
    assert(0);
  }
}

static void *realloc_fun(alloc_t *alloc, void *old_ptr, size_t old_size, size_t new_size) {
  switch (alloc->type) {
  case alloc_os:
    assert(alloc->heap == NULL);
    return realloc(old_ptr, new_size);
  case alloc_heap:
    assert(alloc->heap != NULL);
    {
      void *new_ptr;

      new_ptr = mem_heap_alloc((mem_heap_t *)alloc->heap, new_size);
      memcpy(new_ptr, old_ptr, old_size);

      return new_ptr;
    }
  default:
    assert(0);
  }
}

alloc_t *alloc_create_heap() {
  mem_heap_t *heap;
  alloc_t *alloc;

  heap = mem_heap_create(0);

  alloc = mem_heap_alloc(heap, sizeof(alloc_t));

  alloc->type = alloc_heap;
  alloc->heap = heap;
  alloc->malloc = malloc_fun;
  alloc->free = free_fun;
  alloc->realloc = realloc_fun;

  return alloc;
}

alloc_t *alloc_create_os() {
  alloc_t *alloc;

  alloc = malloc(sizeof(alloc_t));

  alloc->type = alloc_os;
  alloc->heap = NULL;
  alloc->malloc = malloc_fun;
  alloc->free = free_fun;
  alloc->realloc = realloc_fun;

  return alloc;
}

void alloc_free(alloc_t *alloc) {
  switch (alloc->type) {
  case alloc_os:
    assert(alloc->heap == NULL);
    free(alloc);
    break;
  case alloc_heap:
    assert(alloc->heap != NULL);
    mem_heap_free((mem_heap_t *)alloc->heap);
    break;
  default:
    assert(0);
  }
}
