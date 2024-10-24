/*

  vm.h

  stack based cpu virtual machine

  Copyright (c) 2024 yuesong-feng 冯岳松

*/
#ifndef VM_H
#define VM_H
#include "byte.h"
#include "mem.h"
#include <assert.h>

#define VM_WORD_SIZE sizeof(uintptr_t)
#define VM_STACK_SIZE 64
typedef uintptr_t vm_ptr_t;

typedef struct vm_stkfrm_t {
  vm_ptr_t *bp;
  uint32_t *off_locals;
  uint32_t *off_args;
  uint32_t local_space;
  uint32_t arg_space;
} vm_stkfrm_t;

typedef struct vm_t {
  mem_heap_t *heap; //mem heap
  vm_ptr_t *top;     // stack top
  vm_ptr_t *bottom; // stack pointer
  uint32_t stack_size;
  vm_ptr_t *ip; // instruction pointer
  vm_stkfrm_t *stkfrm;
} vm_t;

static inline vm_stkfrm_t *vm_stkfrm_create(vm_t *vm) {
  vm->stkfrm = (vm_stkfrm_t *)mem_heap_alloc(vm->heap, sizeof(vm_stkfrm_t));
  return vm->stkfrm;
}
static inline vm_t *vm_create() {
  vm_t* vm = (vm_t*)mem_alloc(sizeof(vm_t));
  vm->heap = mem_heap_create(0);
  vm->stack_size = VM_STACK_SIZE;
  vm->bottom = (vm_ptr_t*)mem_heap_zalloc(vm->heap, vm->stack_size * VM_WORD_SIZE);
  vm->top = vm->bottom + vm->stack_size - 1;
  vm->stkfrm = vm_stkfrm_create(vm);
  return vm;
}
static inline void vm_destroy(vm_t *vm) {
}
static inline byte *vm_stk_top(vm_t *vm) {
  return (byte *)vm->top;
}
static inline void *vm_stk_push(vm_t *vm, size_t size) {
  assert(vm->bottom + size < vm->top);
  vm->top -= size;
  assert(vm->bottom < vm->top);
  return (byte *)vm->top;
}
static inline void *vm_stk_pop(vm_t *vm, size_t size) {
  byte *ret;
  assert(vm->top + size < vm->bottom + vm->stack_size);
  ret = (byte *)vm->top;
  vm->top += size;
  return ret;
}
static inline void *vm_arg_loc(vm_t *vm, uint32_t argid) {
  return vm->stkfrm->bp + (uintptr_t)vm->stkfrm->local_space + (uintptr_t)vm->stkfrm->off_args[argid];
}
static inline void *vm_var_loc(vm_t *vm, uint32_t varid) {
  return vm->stkfrm->bp + (uintptr_t)vm->stkfrm->local_space - (uintptr_t)vm->stkfrm->off_locals[varid];
}
static inline vm_t *vm_set_next_ip(vm_t *vm, size_t len) {
  vm->ip += len;
  return vm;
}

enum {
  op_nop,
  op_hlt,
  op_load_arg,
  op_load_loc,
  op_store_arg,
  op_store_loc,
  op_add
};

typedef uint16_t opcode_t;

typedef struct op_t op_t;
struct op_t {
  opcode_t opcode; // 2 bytes
  LIST_NODE(op_t) link;
};

typedef LIST(op_t) op_lst_t;

typedef struct op_hdr_t op_hdr_t;
struct op_hdr_t {
  opcode_t opcode;
  LIST_NODE(op_hdr_t) link;
};

typedef struct op_load_t {
  op_hdr_t hdr;
  uint32_t id;
  uint32_t len;
} op_load_t;

typedef struct op_store_t {
  op_hdr_t hdr;
  uint16_t len;
} op_store_t;

static inline void op_pack(op_lst_t *ops, byte *bytecode) {
  op_t *op;
  LIST_FOREACH(link, op, *ops) {
    write2b(bytecode, op->opcode);
    switch (op->opcode) {
    case op_nop:
    case op_hlt:
      break;
    case op_load_arg:
    case op_load_loc:
      write4b(bytecode + 2, ((op_load_t*)op)->id);
      write2b(bytecode + 6, ((op_load_t*)op)->len);
      break;
    case op_store_arg:
    case op_store_loc:
      write2b(bytecode + 2, ((op_store_t*)op)->len);
      break;
    default:
      ast(false);
    }
  }
}

static inline vm_t *op_nop_exec(vm_t *vm) {
  return vm_set_next_ip(vm, 2);
}
static inline vm_t *op_load_exec(vm_t *vm, uint16_t opcode, uint32_t id, uint16_t len) {
  void *p = NULL;
  if (opcode == op_load_arg)
    p = vm_arg_loc(vm, id);
  else if (opcode == op_load_loc)
    p = vm_var_loc(vm, id);
  assert(p != NULL);
  void *p2 = vm_stk_push(vm, len);
  memcpy(p2, p, len * VM_WORD_SIZE);
  return vm_set_next_ip(vm, 8);
}
static inline vm_t *op_store_exec(vm_t *vm, uint16_t opcode, uint16_t len) {
  void *opr1 = vm_stk_pop(vm, len);
  void *opr2 = vm_stk_pop(vm, 1);
  void *p = NULL;
  if (opcode == op_store_arg)
    p = vm_arg_loc(vm, *(uint32_t*)opr2);
  else if (opcode = op_store_loc)
    p = vm_var_loc(vm, *(uint32_t*)opr2);
  assert(p != NULL);
  memcpy(p, opr1, len * VM_WORD_SIZE);
  return vm_set_next_ip(vm, 4);
}

static inline vm_t *vm_exec(vm_t *vm) {
  uint16_t opcode = read2b(vm->ip);
  switch (opcode) {
  case op_nop:
    return vm_set_next_ip(vm, 2);
  case op_hlt:
    vm->ip = NULL;
    return NULL;
  case op_load_arg:
  case op_load_loc:
    return op_load_exec(vm, opcode, read4b(vm->ip + 2), read2b(vm->ip + 6));
  case op_store_loc:
  case op_store_arg:
    return op_store_exec(vm, opcode, read2b(vm->ip + 2));
  default:
    break;
  }
  return vm;
}

#endif