#include "mem.h"
#include "heap.h"
#include <stdlib.h>
#include <assert.h>

mem_t *mem_create(int type) {
  mem_t *mem;
  assert(type == mem_type_os || type == mem_type_heap);
  if (type == mem_type_os) {
    mem = malloc(sizeof(mem_t));
    mem->obj = NULL;
  } else if (type == mem_type_heap) {
    mem_heap_t *heap = mem_heap_create(sizeof(mem_t));
    mem = mem_heap_alloc(heap, sizeof(mem_t));
    mem->obj = heap;
  }
  mem->type = type;
}

void mem_destroy(mem_t *mem) {
  if (mem->type == mem_type_os)
    free(mem);
  else if (mem->type == mem_type_heap)
    mem_heap_free(mem->obj);
}

void *mem_alloc(mem_t *mem, size_t size) {
  if (mem->type == mem_type_os)
    return malloc(size);
  else if (mem->type == mem_type_heap)
    return mem_heap_alloc(mem->obj, size);
}

void *mem_free(mem_t *mem, void *ptr) {
  if (mem->type == mem_type_os)
    free(ptr);
}