#include "vec.h"
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

size_t max(size_t a, size_t b) {
    return (a > b) ? a : b;
}

void swap(void *a, void *b, size_t sizeof_type) {
    char temp[sizeof_type];
    memcpy(temp, a, sizeof_type);
    memcpy(a, b, sizeof_type);
    memcpy(b, temp, sizeof_type);
}

void fill(void *first, void *last, void *value, size_t sizeof_value) {
    if (sizeof_value == 1) {
        memset(first, *(unsigned char *)value, (char *)last - (char *)first);
        return;
    }
    for ( ; first != last; first = (char*)first + sizeof_value)
        memcpy(first, value, sizeof_value);
}

void *fill_n(void *first, size_t count, void *value, size_t sizeof_value) {
    for ( ; count > 0; --count, first = (char*)first + sizeof_value)
        memcpy(first, value, sizeof_value);
    return first;
}

void construct(void *pos, void *value, size_t sizeof_value) {
    if (value)
        memcpy(pos, value, sizeof_value);
}

void *copy(void *first, void *last, void *result, size_t sizeof_value) {
    for ( ; first != last; result = (char*)result + sizeof_value, first = (char*)first + sizeof_value)
        memcpy(result, first, sizeof_value);
    return result;
}

void *move(void *first, void *last, void *result, size_t sizeof_value) {
    memmove(result, first, (char*)last - (char*)first);
    return (char*)result + ((char*)last - (char*)first);
}

void *copy_backward(void *first, void *last, void *result, size_t sizeof_value) {
    while (first != last) {
        result = (char*)result - sizeof_value;
        last = (char*)last - sizeof_value;
        memcpy(result, last, sizeof_value);
    }
    return result;
}

void *move_backward(void *first, void *last, void *result, size_t sizeof_value) {
    ptrdiff_t diff = (char*)last - (char*)first;
    memmove((char*)result - diff, first, diff);
    return (char*)result - diff;
}

void vector_insert_aux(vector_t *vec, void *pos, void *value) {
    if (vec->finish != vec->end_of_storage) {
        construct(vec->finish, (char*)vec->finish - vec->sizeof_value, vec->sizeof_value);
        vec->finish = (char*)vec->finish + vec->sizeof_value;
        copy_backward(pos, (char*)vec->finish - 2 * vec->sizeof_value, (char*)vec->finish - vec->sizeof_value, vec->sizeof_value);
        memcpy(pos, value, vec->sizeof_value);
    } else {
        size_t old_size = vector_size(vec);
        size_t len = old_size != 0 ? 2 * old_size : 1;
        void *new_start = malloc(len * vec->sizeof_value);
        void *new_finish = new_start;
        new_finish = copy(vec->start, pos, new_start, vec->sizeof_value);
        construct(new_finish, value, vec->sizeof_value);
        new_finish = (char*)new_finish + vec->sizeof_value;
        new_finish = copy(pos, vec->finish, new_finish, vec->sizeof_value);
        free(vec->start);
        vec->start = new_start;
        vec->finish = new_finish;
        vec->end_of_storage = (char*)new_start + len * vec->sizeof_value;
    }
}

void *allocate_and_copy(size_t len, void *first, void *last, size_t sizeof_value) {
    void *result = malloc(len);
    assert(result != NULL);
    copy(first, last, result, sizeof_value);
    return result;
}

void vector_init(vector_t *vec, size_t sizeof_value) {
    vec->start = NULL;
    vec->finish = NULL;
    vec->end_of_storage = NULL;
    vec->sizeof_value = sizeof_value;
}

void vector_init1(vector_t *vec, size_t sizeof_value, size_t count) {
    vec->start = malloc(count * sizeof_value);
    assert(vec->start != NULL);
    vec->finish = vec->start;
    vec->end_of_storage = (char*)vec->start + count * sizeof_value;
    vec->sizeof_value = sizeof_value;
}

void vector_init2(vector_t *vec, size_t sizeof_value, size_t count, void *value) {
    vector_init1(vec, sizeof_value, count);
    vec->finish = fill_n(vec->start, count, value, sizeof_value);
}

void vector_destroy(vector_t *vec) {
    if (vec->start)
        free(vec->start);
}

void *vector_at(vector_t *vec, size_t pos) {
    assert(pos < vector_size(vec));
    return (char*)vector_begin(vec) + pos * vec->sizeof_value;
}

void *vector_front(vector_t *vec) {
    return vector_begin(vec);
}

void *vector_back(vector_t *vec) {
    return (char*)vector_end(vec) - vec->sizeof_value;
}

void *vector_data(vector_t *vec) {
    return vec->start;
}

void *vector_begin(vector_t *vec) {
    return vec->start;
}

void *vector_end(vector_t *vec) {
    return vec->finish;
}

bool vector_empty(vector_t *vec) {
    return vector_begin(vec) == vector_end(vec);
}

size_t vector_size(vector_t *vec) {
    return (size_t)((char*)vector_end(vec) - (char*)vector_begin(vec)) / vec->sizeof_value;
}

size_t vector_max_size(vector_t *vec) {
    return (size_t)(-1) / vec->sizeof_value;
}

void vector_reserve(vector_t *vec, size_t new_cap) {
    if (vector_capacity(vec) < new_cap) {
        size_t old_size = vector_size(vec);
        void *tmp = allocate_and_copy(new_cap * vec->sizeof_value, vec->start, vec->finish, vec->sizeof_value);
        free(vec->start);
        vec->start = tmp;
        vec->finish = (char*)vec->start + old_size * vec->sizeof_value;
        vec->end_of_storage = (char*)vec->start + new_cap * vec->sizeof_value;
    }
}

size_t vector_capacity(vector_t *vec) {
    return (size_t)((char*)vec->end_of_storage - (char*)vector_begin(vec)) / vec->sizeof_value;
}

void vector_clear(vector_t *vec) {
    vector_erase2(vec, vector_begin(vec), vector_end(vec));
}

void *vector_insert(vector_t *vec, void * pos, void *value) {
    size_t n = (char*)pos - (char*)vector_begin(vec);
    if (vec->finish != vec->end_of_storage && pos == vector_end(vec)) {
        construct(vec->finish, value, vec->sizeof_value);
        vec->finish = (char*)vec->finish + vec->sizeof_value;
    } else 
        vector_insert_aux(vec, pos, value);
    return (char*)vector_begin(vec) + n * vec->sizeof_value;
}

void vector_insert1(vector_t *vec, void * pos, size_t count, void *value) {
    if (count != 0) {
        if (((char*)vec->end_of_storage - (char*)vec->finish) / vec->sizeof_value >= count) {
            size_t elems_after = ((char*)vec->finish - (char*)pos) / vec->sizeof_value;
            void *old_finish = vec->finish;
            if (elems_after > count) {
                copy((char*)vec->finish - count * vec->sizeof_value, vec->finish, vec->finish, vec->sizeof_value);
                vec->finish = (char*)vec->finish + count * vec->sizeof_value;
                copy_backward(pos, (char*)old_finish - count * vec->sizeof_value, old_finish, vec->sizeof_value);
                fill(pos, (char*)pos + count * vec->sizeof_value, value, vec->sizeof_value);
            } else {
                fill_n(vec->finish, count - elems_after, value, vec->sizeof_value);
                vec->finish = (char*)vec->finish + (count - elems_after) * vec->sizeof_value;
                copy(pos, old_finish, vec->finish, vec->sizeof_value);
                vec->finish = (char*)vec->finish + elems_after * vec->sizeof_value;
                fill(pos, old_finish, value, vec->sizeof_value);
            }
        } 
    } else {
        size_t old_size = vector_size(vec);
        size_t len = old_size + max(old_size, count);
        void *new_start = malloc(len * vec->sizeof_value);
        assert(new_start != NULL);
        void *new_finish = new_start;
        new_finish = copy(vec->start, pos, new_start, vec->sizeof_value);
        new_finish = fill_n(new_finish, count, value, vec->sizeof_value);
        new_finish = copy(pos, vec->finish, new_finish, vec->sizeof_value);
        free(vec->start);
        vec->start = new_start;
        vec->finish = new_finish;
        vec->end_of_storage = (char*)new_start + len * vec->sizeof_value;
    }
}

void *vector_erase(vector_t *vec, void * pos) {
    if ((char*)pos + vec->sizeof_value != vector_end(vec))
        copy((char*)pos + vec->sizeof_value, vec->finish, pos, vec->sizeof_value);
    vec->finish = (char*)vec->finish - vec->sizeof_value;
    return pos;
}

void *vector_erase2(vector_t *vec, void * first, void * last) {
    void *i = copy(last, vec->finish, first, vec->sizeof_value);
    vec->finish = (char*)vec->finish - ((char*)last - (char*)first);
    return first;
}

void vector_push_back(vector_t *vec, void *value) {
    if (vec->finish != vec->end_of_storage) {
        construct(vec->finish, value, vec->sizeof_value);
        vec->finish = (char*)vec->finish + vec->sizeof_value;
    }
    else
        vector_insert_aux(vec, vector_end(vec), value);
}

void vector_pop_back(vector_t *vec) {
    vec->finish = (char*)vec->finish - vec->sizeof_value;
}

void vector_resize(vector_t *vec, size_t count) {
    vector_resize1(vec, count, NULL);
}

void vector_resize1(vector_t *vec, size_t count, void *value) {
    if (count < vector_size(vec))
        vector_erase2(vec, (char*)vector_begin(vec) + count * vec->sizeof_value, vector_end(vec));
    else
        vector_insert1(vec, vector_end(vec), count - vector_size(vec), value);
}

void vector_swap(vector_t *vec, vector_t *other) {
    swap(&vec->start, &other->start, sizeof(void *));
    swap(&vec->finish, &other->finish, sizeof(void *));
    swap(&vec->end_of_storage, &other->end_of_storage, sizeof(void *));
}