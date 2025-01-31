/**
 * @file byte.c
 * @date 2025-01-31
 * @author yuesong-feng
 */
#include "byte.h"
#include <assert.h>

void write1b(byte *b, uint8_t n) {
  b[0] = (byte)n;
}

uint8_t read1b(const byte *b) {
  return (uint8_t)b[0];
}

void write2b(byte *b, uint16_t n) {
  b[0] = (byte)(n >> 8);
  b[1] = (byte)n;
}

uint16_t read2b(const byte *b) {
  return ((uint16_t)b[0] << 8) |
         (uint16_t)b[1];
}

void write4b(byte *b, uint32_t n) {
  b[0] = (byte)(n >> 24);
  b[1] = (byte)(n >> 16);
  b[2] = (byte)(n >> 8);
  b[3] = (byte)n;
}

uint32_t read4b(const byte *b) {
  return ((uint32_t)b[0] << 24) |
         ((uint32_t)b[1] << 16) |
         ((uint32_t)b[2] << 8) |
         (uint32_t)b[3];
}

void write8b(void *b, uint64_t n) {
  write4b((byte *)b, (uint32_t)(n >> 32));
  write4b((byte *)b + 4, (uint32_t)n);
}

uint64_t read8b(const byte *b) {
  uint64_t n;
  n = (uint64_t)read4b(b) << 32;
  n |= (uint64_t)read4b(b + 4);
  return n;
}