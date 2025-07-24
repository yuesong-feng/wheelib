/**
 * @file vec.c
 * @date 2025-01-29
 * @author yuesong-feng
 */

 /*
#include "vec_old.h"
#include "byte.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define VEC_OFFSET(vec, i) (vec->sizeof_value * i)

vector_t *vector_create(size_t sizeof_value, size_t size) {
  vector_t *vec;

  assert(size > 0);

  vec = malloc(sizeof(*vec));
  assert(vec != NULL);

  vec->used = 0;
  vec->total = size;
  vec->sizeof_value = sizeof_value;
  vec->data = malloc(vec->sizeof_value * size);
  assert(vec->data);

  return vec;
}

void vector_free(vector_t *vec) {
  free(vec->data);
  free(vec);
}

void *vector_push(vector_t *vec, const void *elem) {
  void *last;

  if (vec->used >= vec->total) {
    vector_resize(vec);
  }

  last = (byte *)vec->data + VEC_OFFSET(vec, vec->used);

  memset(last, 0, vec->sizeof_value);

  if (elem) {
    memcpy(last, elem, vec->sizeof_value);
  }

  ++vec->used;

  return last;
}

void *vector_pop(vector_t *vec) {
  void *elem;
  assert(vec->used > 0);
  elem = vector_last(vec);
  --vec->used;
  return elem;
}

void *vector_remove(vector_t *vec, const void *elem) {
  void *current = NULL;
  void *next;
  size_t i;
  size_t old_used_count = vec->used;

  for (i = 0; i < vec->used; ++i) {
    current = vector_get(vec, i);

    if (*(void **)current == elem) {
      if (i == vec->used - i) {
        return vector_pop(vec);
      }

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

  vec->data = realloc(vec->data, new_size);
  assert(vec->data);

  vec->total = new_total;
}

bool vector_is_empty(const vector_t *vec) {
  return vector_sizexx(vec) == 0;
}

void *vector_get(const vector_t *vec, size_t n) {
  assert(n < vec->used);
  return (byte *)vec->data + VEC_OFFSET(vec, n);
}

void *vector_get_last(vector_t *vec) {
  assert(vec->used > 0);
  return (byte *)vector_get(vec, vec->used - 1);
}

void vector_set(vector_t *vec, size_t n, void *elem) {
  void *slot;
  assert(n < vec->used);
  slot = (byte *)vec->data + VEC_OFFSET(vec, n);
  memcpy(slot, elem, vec->sizeof_value);
}

void vector_reset(vector_t *vec) {
  vec->used = 0;
}

void *vector_last(vector_t *vec) {
  assert(vector_sizexx(vec) > 0);
  return vector_get(vec, vector_sizexx(vec) - 1);
}

void vector_sort(vector_t *vec, int (*cmpare)(const void *, const void *)) {
  qsort(vec->data, vec->used, vec->sizeof_value, cmpare);
}
*/