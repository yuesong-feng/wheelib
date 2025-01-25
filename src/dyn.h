#ifndef DYN_H
#define DYN_H
#include "byte.h"
#include "lst.h"

#define DYN_ARRAY_DATA_SIZE 512

typedef struct dyn_block_t dyn_block_t;
struct dyn_block_t {
    size_t used;
    byte data[DYN_ARRAY_DATA_SIZE];
    LIST(dyn_block_t) base;
    LIST_NODE(dyn_block_t) list;
};

typedef struct dyn_block_t dyn_array_t;

dyn_array_t *dyn_array_create(dyn_array_t *arr);

void dyn_array_free(dyn_array_t* arr);

byte *dyn_array_open(dyn_array_t *arr, size_t size);

void dyn_array_close(dyn_array_t* arr, const byte *ptr);

void *dyn_array_push(dyn_array_t *arr, size_t size);

void *dyn_array_get_element(const dyn_array_t *arr, size_t pos);

size_t dyn_array_get_data_size(const dyn_array_t *arr);


#endif