#ifndef ALLOC_H
#define ALLOC_H
#include <stddef.h>

typedef struct alloc_t alloc_t;

typedef void *(*mem_alloc_t)(alloc_t *alloc, size_t size);

typedef void (*mem_free_t)(alloc_t *alloc, void *ptr);

typedef void *(*mem_realloc_t)(alloc_t *alloc, void *old_ptr, size_t old_size, size_t new_size);

struct alloc_t {
  int type;
  void *heap;
  mem_alloc_t malloc;
  mem_free_t free;
  mem_realloc_t realloc;
};

alloc_t *alloc_create_heap();

alloc_t *alloc_create_os();

void alloc_free(alloc_t *alloc);

#endif