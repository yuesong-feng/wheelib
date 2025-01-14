#include "vec.h"
#include "byte.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

vector_t *vector_create(alloc_t *alloc, size_t sizeof_value, size_t size) {
  vector_t *vec;

  vec = alloc->malloc(alloc, sizeof(vector_t));

  vec->alloc = alloc;
  vec->used = 0;
  vec->total = size;
  vec->sizeof_value = sizeof_value;

  vec->data = alloc->malloc(alloc, vec->sizeof_value * size);

  return vec;
}

void vector_free(vector_t *vec) {
  vec->alloc->free(vec->alloc, vec->data);
  vec->alloc->free(vec->alloc, vec);
}

#define VEC_OFFSET(vec, nth) (vec->sizeof_value * nth)

void *vector_push(vector_t *vec, const void *elem) {
  void *last;

  if (vec->used >= vec->total) {
    vector_resize(vec);
  }

  last = (byte *)vec->data + VEC_OFFSET(vec, vec->used);

  if (elem)
    memcpy(last, elem, vec->sizeof_value);

  ++vec->used;

  return last;
}

void *vector_pop(vector_t *vec) {
  void *elem;

  assert(vec->used > 0);

  elem = vector_get_last(vec);
  --vec->used;

  return elem;
}

void *vector_remove(vector_t *vec, const void *elem) {
  size_t i;
  void *current = NULL;
  void *next;
  size_t old_used_count = vec->used;

  for (i = 0; i < vec->used; ++i) {
    current = vector_get(vec, i);

    if (*(void **)current == elem) {
      if (i == vec->used - 1)
        return vector_pop(vec);

      next = vector_get(vec, i + 1);
      memmove(current, next, vec->sizeof_value * (vec->used - i - 1));
      --vec->used;
      break;
    }
  }

  return old_used_count != vec->used ? current : NULL;
}

size_t vector_size(const vector_t *vec) {
  return vec->used;
}

void vector_resize(vector_t *vec) {
  size_t new_total = vec->total * 2;
  size_t old_size = vec->used * vec->sizeof_value;
  size_t new_size = new_total * vec->sizeof_value;

  vec->data = vec->alloc->realloc(vec->alloc, vec->data, old_size, new_size);

  vec->total = new_total;
}

bool vector_is_empty(const vector_t *vec) {
  return vector_size(vec) == 0;
}

void *vector_get(vector_t *vec, size_t nth) {
  assert(nth < vec->used);
  return (byte *)vec->data + VEC_OFFSET(vec, nth);
}

void *vector_get_last(vector_t *vec) {
  assert(vec->used > 0);
  return vector_get(vec, vector_size(vec) - 1);
}

void vector_set(vector_t *vec, size_t nth, void *elem) {
  void *slot;

  assert(nth < vec->used);

  slot = (byte *)vec->data + VEC_OFFSET(vec, nth);
  memcpy(slot, elem, vec->sizeof_value);
}

void vector_reset(vector_t *vec) {
  vec->used = 0;
}

typedef int (*compare_t)(const void *, const void *);

void vector_sort(vector_t *vec, compare_t compare) {
  qsort(vec->data, vec->used, vec->sizeof_value, compare);
}