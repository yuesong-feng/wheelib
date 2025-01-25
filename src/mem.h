#ifndef MEM_H
#define MEM_H
#include <stddef.h>

enum mem_type {
  mem_type_os,
  mem_type_heap
};

typedef struct mem_t mem_t;
struct mem_t {
  int type;
  void *obj;
};

mem_t *mem_create(int type);

void mem_destroy(mem_t *mem);

void *mem_alloc(mem_t *mem, size_t size);

void mem_free(mem_t *mem, void *ptr);

#endif