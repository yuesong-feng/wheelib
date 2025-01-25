#include "dyn.h"
#include <assert.h>

dyn_array_t *dyn_array_create(dyn_array_t *arr){

}

void dyn_array_free(dyn_array_t* arr){

}

byte *dyn_array_open(dyn_array_t *arr, size_t size){
    // dyn_block_t* block;
    // assert(size <= DYN_ARRAY_DATA_SIZE);

    // block = arr;

    // if (block->used + size > DYN_ARRAY_DATA_SIZE) {
    //     block = dyn_array_get_last_block(arr);
        
    //     if (block->used + size > DYN_ARRAY_DATA_SIZE) {
    //         block = dyn_array_add_block(arr);
    //     }
    // }

    // assert(block->used + size <= DYN_ARRAY_DATA_SIZE);

    // return block->data + block->used;
}

void dyn_array_close(dyn_array_t* arr, const byte *ptr) {
    // dyn_block_t *block;

    // block = dyn_array_get_last_block(arr);

    // block->used = ptr - block->data;

    // assert(block->used <= DYN_ARRAY_DATA_SIZE);
}

void *dyn_array_push(dyn_array_t *arr, size_t size) {

}