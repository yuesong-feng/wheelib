#ifndef VEC_H
#define VEC_H
#include "alloc.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct vector_t vector_t;
struct vector_t {
  alloc_t *alloc;
  void *data;
  size_t used;
  size_t total;
  size_t sizeof_value;
};

vector_t *vector_create(alloc_t *alloc, size_t sizeof_value, size_t size);

void vector_free(vector_t *vec);

void *vector_push(vector_t *vec, const void *elem);

void *vector_pop(vector_t *vec);

void *vector_remove(vector_t *vec, const void *elem);

size_t vector_size(const vector_t *vec);

void vector_resize(vector_t *vec);

bool vector_is_empty(const vector_t *vec);

void *vector_get(vector_t *vec, size_t nth);

void *vector_get_last(vector_t *vec);

void vector_set(vector_t *vec, size_t nth, void *elem);

void vector_reset(vector_t *vec);

typedef int (*compare_t)(const void *, const void *);

void vector_sort(vector_t *vec, compare_t compare);

#endif