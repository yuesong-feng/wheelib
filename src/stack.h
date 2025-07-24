#ifndef STACK_H
#define STACK_H
#include "vec.h"

typedef vector_t stack_t;

void stack_init(stack_t *stk, size_t sizeof_value);

void stack_destroy(stack_t *stk);

void *stack_top(stack_t *stk);

bool stack_empty(stack_t *stk);

size_t stack_size(stack_t *stk);

void stack_push(stack_t *stk, void *value);

void stack_pop(stack_t *stk);

void stack_swap(stack_t *stk, stack_t *other);

#endif