/*

  byte.h

  byte utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/06

*/
#ifndef BYTE_H
#define BYTE_H
#include "basic.h"  // for halt_if
#include <stdint.h> // for (u)int(n)_t
#include <stdbool.h> //for true

typedef unsigned char byte;

#ifdef __BYTE_ORDER__
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        #define WLIB_LITTLE_ENDIAN
    #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        #define WLIB_BIG_ENDIAN
    #endif
#endif

static inline void
write1b(void *dest, uint8_t v)
{
    *(uint8_t *)dest = v;
}

static inline void
write2b(void *dest, uint16_t v)
{
#if defined(WLIB_LITTLE_ENDIAN)
    *(uint16_t *)dest = v;
#elif defined(WLIB_BIG_ENDIAN)
    write1b(dest, (uint8_t)(v & 0xFF));
    write1b((byte *)dest + 1, (uint8_t)((v >> 8) & 0xFF));
#else
    halt_if(true);
#endif
}

static inline void
write4b(void *dest, uint32_t v)
{
#if defined(WLIB_LITTLE_ENDIAN)
    *(uint32_t *)dest = v;
#elif defined(WLIB_BIG_ENDIAN)
    write2b(dest, (uint16_t)(v & 0xFFFF));
    write2b((byte *)dest + 2, (uint16_t)((v >> 16) & 0xFFFF));
#else
    halt_if(true);
#endif
}

static inline void
write8b(void *dest, uint64_t v)
{
#if defined(WLIB_LITTLE_ENDIAN)
    *(uint64_t *)dest = v;
#elif defined(WLIB_BIG_ENDIAN)
    write4b(dest, (uint32_t)(v & 0xFFFFFFFF));
    write4b((byte *)dest + 4, (uint32_t)((v >> 32) & 0xFFFFFFFF));
#else
    halt_if(true);
#endif
}

static inline uint8_t
read1b(void *src)
{
    return *(uint8_t *)src;
}

static inline uint16_t
read2b(void *src)
{
#if defined(WLIB_LITTLE_ENDIAN)
    return *(uint16_t *)src;
#elif defined(WLIB_BIG_ENDIAN)
    uint16_t  a, b;
    a = read1b(src);
    b = read1b((byte *)src + 1);
    return (b << 8) + a;
#else
    halt_if(true);
#endif
}

static inline uint32_t
read4b(void *src)
{
#if defined(WLIB_LITTLE_ENDIAN)
    return *(uint32_t *)src;
#elif defined(WLIB_BIG_ENDIAN)
    uint32_t  a, b;
    a = read2b(src);
    b = read2b((byte *)src + 2);
    return (b << 16) + a;
#else
    halt_if(true);
#endif
}

static inline uint64_t
read8b(void *src)
{
#if defined(WLIB_LITTLE_ENDIAN)
    return *(uint64_t *)src;
#elif defined(WLIB_BIG_ENDIAN)
    uint64_t  a, b, res;
    a = read4b(src);
    b = read4b((byte *)src + 4);
    res = b << 32;
    res += a;
    return res;
#else
    halt_if(true);
#endif
}

#endif