/*

  byte.h

  byte utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/06

  Version 1.1 2024/09/19 add mach_* from Innobase
*/
#ifndef BYTE_H
#define BYTE_H
#include "basic.h"   // for ast
#include <stdbool.h> //for true
#include <stdint.h>  // for (u)int(n)_t

typedef unsigned char byte;

// haven't cover all platform yet, linux and macos work well
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  #define WLIB_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  #define WLIB_BIG_ENDIAN
#endif

static inline void write1b(void *dest, uint8_t v) {
  *(uint8_t *)dest = v;
}

static inline void write2b(void *dest, uint16_t v) {
#if defined(WLIB_LITTLE_ENDIAN)
  *(uint16_t *)dest = v;
#elif defined(WLIB_BIG_ENDIAN)
  write1b(dest, (uint8_t)(v & 0xFF));
  write1b((byte *)dest + 1, (uint8_t)((v >> 8) & 0xFF));
#else
  ast(false);
#endif
}

static inline void write4b(void *dest, uint32_t v) {
#if defined(WLIB_LITTLE_ENDIAN)
  *(uint32_t *)dest = v;
#elif defined(WLIB_BIG_ENDIAN)
  write2b(dest, (uint16_t)(v & 0xFFFF));
  write2b((byte *)dest + 2, (uint16_t)((v >> 16) & 0xFFFF));
#else
  ast(false);
#endif
}

static inline void write8b(void *dest, uint64_t v) {
#if defined(WLIB_LITTLE_ENDIAN)
  *(uint64_t *)dest = v;
#elif defined(WLIB_BIG_ENDIAN)
  write4b(dest, (uint32_t)(v & 0xFFFFFFFF));
  write4b((byte *)dest + 4, (uint32_t)((v >> 32) & 0xFFFFFFFF));
#else
  ast(false);
#endif
}

static inline uint8_t read1b(void *src) {
  return *(uint8_t *)src;
}

static inline uint16_t read2b(void *src) {
#if defined(WLIB_LITTLE_ENDIAN)
  return *(uint16_t *)src;
#elif defined(WLIB_BIG_ENDIAN)
  uint16_t a, b;
  a = read1b(src);
  b = read1b((byte *)src + 1);
  return (b << 8) + a;
#else
  ast(false);
#endif
}

static inline uint32_t read4b(void *src) {
#if defined(WLIB_LITTLE_ENDIAN)
  return *(uint32_t *)src;
#elif defined(WLIB_BIG_ENDIAN)
  uint32_t a, b;
  a = read2b(src);
  b = read2b((byte *)src + 2);
  return (b << 16) + a;
#else
  ast(false);
#endif
}

static inline uint64_t read8b(void *src) {
#if defined(WLIB_LITTLE_ENDIAN)
  return *(uint64_t *)src;
#elif defined(WLIB_BIG_ENDIAN)
  uint64_t a, b, res;
  a = read4b(src);
  b = read4b((byte *)src + 4);
  res = b << 32;
  res += a;
  return res;
#else
  ast(false);
#endif
}

static inline void mach_write_to_1(byte *b, uint8_t n) {
  b[0] = (byte)n;
}

static inline uint8_t mach_read_from_1(const byte *b) {
  return ((uint8_t)(b[0]));
}

static inline void mach_write_to_2(byte *b, uint16_t n) {
  b[0] = (byte)(n >> 8);
  b[1] = (byte)(n);
}

static inline uint16_t mach_read_from_2(const byte *b) {
  return (((uint16_t)(b[0]) << 8) | (uint16_t)(b[1]));
}

static inline void mach_write_to_4(byte *b, uint32_t n) {
  b[0] = (byte)(n >> 24);
  b[1] = (byte)(n >> 16);
  b[2] = (byte)(n >> 8);
  b[3] = (byte)n;
}

static inline uint32_t mach_read_from_4(const byte *b) {
  return (((uint32_t)(b[0]) << 24) |
          ((uint32_t)(b[1]) << 16) |
          ((uint32_t)(b[2]) << 8) |
          (uint32_t)(b[3]));
}

static inline void mach_write_to_8(void *b, uint64_t n) {
  mach_write_to_4((byte *)(b), (uint32_t)(n >> 32));
  mach_write_to_4((byte *)(b) + 4, (uint32_t)n);
}

static inline uint64_t mach_read_from_8(const byte *b) {
  uint64_t u64;

  u64 = mach_read_from_4(b);
  u64 <<= 32;
  u64 |= mach_read_from_4(b + 4);

  return (u64);
}

#endif