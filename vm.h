/*

  vm.h

  stack based cpu virtual machine

  Copyright (c) 2024 yuesong-feng 冯岳松

*/
#ifndef VM_H
#define VM_H
#include "byte.h"
#include "mem.h"

#define VM_WORD_SIZE sizeof(uintptr_t)
typedef uintptr_t vm_ptr_t;

typedef struct vm_t {
  vm_ptr_t *sp;     // stack top
  vm_ptr_t *bottom; // stack pointer
  uint32_t stack_size;
  vm_ptr_t *ip; // instruction pointer
} vm_t;

static inline vm_t *vm_create() {
}
static inline void vm_destroy(vm_t *vm) {
}
static inline void *vm_stk_push(vm_t *vm, size_t size) {
}
static inline void *vm_stk_pop(vm_t *vm, size_t size) {
}
static inline void *vm_var_loc(vm_t *vm, byte varid) {
}
static inline void vm_set_next_ip(vm_t *vm, size_t len) {
  vm->ip += len;
}
typedef enum opcode_t {
  op_nop,
  op_load,
  op_store,
  op_add
} opcode_t;

typedef struct op_t op_t;
struct op_t {
  opcode_t opcode; // 4 bytes
  LIST_NODE(op_t) link;
};

typedef LIST(op_t) op_lst_t;

static inline byte op_len(opcode_t opcode) {
  switch (opcode) {
  case op_nop:
    return 2;
  case op_load:
    return 2;
  case op_store:
    return 2;
  default:
    halt_if(true);
  }
}

static inline void op_pack(op_lst_t *ops, byte *bytecode) {
  op_t *op;
  LIST_FOREACH(link, op, *ops) {
    switch (op->opcode) {
    case op_nop:
      write2b(bytecode, 0);
      break;
    case op_load:
      break;
    case op_store:
      break;
    default:
      halt_if(true);
    }
  }
}

static inline vm_t *op_nop_exec(vm_t *vm) {
  vm_set_next_ip(vm, op_len(op_nop));
}
static inline vm_t *op_load_exec(vm_t *vm) {
  vm_set_next_ip(vm, op_len(op_load));
}
static inline vm_t *op_store_exec(vm_t *vm) {
  vm_set_next_ip(vm, op_len(op_store));
}

static inline vm_t *vm_exec(vm_t *vm) {
  byte opcode;

  opcode = read1b(vm->ip);

  switch (opcode) {
  case op_nop:
    break;
  case op_load:
    return op_load_exec(vm);
  case op_store:
    return op_store_exec(vm);
  default:
    break;
  }
}

#endif