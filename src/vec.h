#ifndef VEC_H
#define VEC_H
#include <stddef.h>

typedef struct vector_t vector_t;
struct vector_t {
    void *start;
    void *finish;
    void *end_of_storage;
    size_t sizeof_value;
};

void vector_init(vector_t *vec, size_t sizeof_value);

void vector_init1(vector_t *vec, size_t sizeof_value, size_t count);

void vector_init2(vector_t *vec, size_t sizeof_value, size_t count, void *value);

void vector_destroy(vector_t *vec);

void *vector_at(vector_t *vec, size_t pos);

void *vector_front(vector_t *vec);

void *vector_back(vector_t *vec);

void *vector_data(vector_t *vec);

void *vector_begin(vector_t *vec);

void *vector_end(vector_t *vec);

bool vector_empty(vector_t *vec);

size_t vector_size(vector_t *vec);

size_t vector_max_size(vector_t *vec);

void vector_reserve(vector_t *vec, size_t new_cap);

size_t vector_capacity(vector_t *vec);

void vector_clear(vector_t *vec);

void *vector_insert(vector_t *vec, void * pos, void *value);

void vector_insert1(vector_t *vec, void * pos, size_t count, void *value);

void *vector_erase(vector_t *vec, void *pos);

void *vector_erase2(vector_t *vec, void * first, void * last);

void vector_push_back(vector_t *vec, void *value);

void vector_pop_back(vector_t *vec);

void vector_resize(vector_t *vec, size_t count);

void vector_resize1(vector_t *vec, size_t count, void *value);

void vector_swap(vector_t *vec, vector_t *other);

#endif