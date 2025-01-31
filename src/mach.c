/**
 * @file mach.c
 * @date 2025-01-29
 * @author yuesong-feng
 */
#include "mach.h"
#include "calc.h"
#include <assert.h>

void mach_write_to_1(byte *b, size_t n) {
  assert(b);
  assert((n | 0xFFUL) <= 0xFFUL);

  b[0] = (byte)n;
}

size_t mach_read_from_1(const byte *b) {
  assert(b);
  return (size_t)b[0];
}

void mach_write_to_2(byte *b, size_t n) {
  assert(b);
  assert((n | 0xFFFFUL) <= 0xFFFFUL);

  b[0] = (byte)(n >> 8);
  b[1] = (byte)n;
}

size_t mach_read_from_2(const byte *b) {
  assert(b);
  return (((size_t)(b[0]) << 8) | (size_t)(b[1]));
}

void mach_write_to_3(byte *b, size_t n) {
  assert(b);
  assert((n | 0xFFFFFFUL) <= 0xFFFFFFUL);

  b[0] = (byte)(n >> 16);
  b[1] = (byte)(n >> 8);
  b[2] = (byte)n;
}

size_t mach_read_from_3(const byte *b) {
  assert(b);
  return (((size_t)(b[0]) << 16) | ((size_t)(b[1]) << 8) | (size_t)(b[2]));
}

void mach_write_to_4(byte *b, size_t n) {
  assert(b);
  b[0] = (byte)(n >> 24);
  b[1] = (byte)(n >> 16);
  b[2] = (byte)(n >> 8);
  b[3] = (byte)n;
}

size_t mach_read_from_4(const byte *b) {
  assert(b);
  return (((size_t)(b[0]) << 24) | ((size_t)(b[1]) << 16) | ((size_t)(b[2]) << 8) | (size_t)(b[3]));
}

void mach_write_to_6(void *b, uint64_t n) {
  assert(b);
  mach_write_to_2(b, (size_t)(n >> 32));
  mach_write_to_4(b + 2, (size_t)n);
}

uint64_t mach_read_from_6(const byte *b) {
  assert(b);
  return uint64_create(mach_read_from_2(b), mach_read_from_4(b + 2));
}

void mach_write_to_7(void *b, uint64_t n) {
  assert(b);
  mach_write_to_3(b, (size_t)(n >> 32));
  mach_write_to_4(b + 3, (size_t)n);
}

uint64_t mach_read_from_7(const byte *b) {
  assert(b);
  return (uint64_create(mach_read_from_3(b), mach_read_from_4(b + 3)));
}

void mach_write_to_8(void *b, uint64_t n) {
  assert(b);
  mach_write_to_4((byte *)(b), (size_t)(n >> 32));
  mach_write_to_4((byte *)(b) + 4, (size_t)n);
}

uint64_t mach_read_from_8(const byte *b) {
  uint64_t n;
  n = ((uint64_t)mach_read_from_4(b)) << 32;
  n |= (uint64_t)mach_read_from_4(b + 4);
  return n;
}