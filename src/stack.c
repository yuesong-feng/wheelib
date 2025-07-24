#include "stack.h"

void stack_init(stack_t *stk, size_t sizeof_value) {
    vector_init(stk, sizeof_value);
}

void stack_destroy(stack_t *stk) {
    vector_destroy(stk);
}

void *stack_top(stack_t *stk) {
    return vector_back(stk);
}

bool stack_empty(stack_t *stk) {
    return vector_empty(stk);
}

size_t stack_size(stack_t *stk) {
    return vector_size(stk);
}

void stack_push(stack_t *stk, void *value) {
    vector_push_back(stk, value);
}

void stack_pop(stack_t *stk) {
    vector_pop_back(stk);
}

void stack_swap(stack_t *stk, stack_t *other) {
    vector_swap(stk, other);
}