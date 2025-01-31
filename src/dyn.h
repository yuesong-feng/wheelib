/**
 * @file dyn.h
 * @date 2025-01-29
 * @author yuesong-feng
 */
#ifndef DYN_H
#define DYN_H
#include "heap.h"
#include "byte.h"
#include "lst.h"

#define	DYN_ARRAY_DATA_SIZE	512

typedef struct dyn_block_t dyn_block_t;
struct dyn_block_t {
    mem_heap_t *heap;
    size_t used;
    byte data[DYN_ARRAY_DATA_SIZE];
    LIST(dyn_block_t) base;
    LIST_NODE(dyn_block_t) list;
};

typedef dyn_block_t dyn_array_t;

dyn_array_t *dyn_array_create(dyn_array_t *arr);

void dyn_array_free(dyn_array_t *arr);

byte *dyn_array_open(dyn_array_t *arr, size_t size);

void dyn_array_close(dyn_array_t *arr, const byte *ptr);

void *dyn_array_push(dyn_array_t *arr, size_t size);

void *dyn_array_get_element(const dyn_array_t *arr, size_t pos);

size_t dyn_array_get_data_size(const dyn_array_t *arr);

#define dyn_array_get_first_block(arr) (arr)

#define dyn_array_get_last_block(arr) ((arr)->heap ? LIST_GET_LAST((arr)->base) : (arr))

#define dyn_array_get_next_block(arr, block) ((arr)->heap ? LIST_GET_NEXT(list, block) : NULL)

#define dyn_array_get_prev_block(arr, block) ((arr)->heap ? LIST_GET_PREV(list, block) : NULL)

size_t dyn_block_get_used(const dyn_block_t *block);

byte *dyn_block_get_data(dyn_block_t *block);

void dyn_push_string(dyn_array_t *arr, const char *str, size_t len);


#endif