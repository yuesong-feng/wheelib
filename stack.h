/*

  stack.h

  stack utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/09 complete stack create/free/pusb_8b/pop_8b

*/
#ifndef STACK_H
#define STACK_H
#include "mem.h" // for mem_heap

typedef struct stack_t {
  mem_heap_t *heap;
  size_t size;
  byte *top;
} stack_t;

static inline stack_t *stack_create() {
  stack_t *stack = (stack_t *)mem_alloc(sizeof(stack_t));
  stack->heap = mem_heap_create(0);
  return stack;
}

static inline void stack_free(stack_t *stack) {
  mem_heap_free(stack->heap);
  mem_free(stack);
}

static inline void stack_push_8b(stack_t *stack, uint64_t val) {
  void *buf = mem_heap_alloc(stack->heap, 8);
  mach_write_to_8(buf, val);
}

static inline uint64_t stack_pop_8b(stack_t *stack) {
  void *buf = mem_heap_get_top(stack->heap, 8);
  uint64_t ret = mach_read_from_8(buf);
  mem_heap_free_top(stack->heap, 8);
  return ret;
}

#endif