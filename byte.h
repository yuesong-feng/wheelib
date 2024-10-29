/*

  byte.h

  byte utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/06

  Version 1.1 2024/09/19 add mach_* from Innobase

  Version 2.0 2024/10/29 rewrite
*/
#ifndef BYTE_H
#define BYTE_H

#include "dbg.h"
#include "calc.h"

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
  wl_a(false);
#endif
}

static inline void write4b(void *dest, uint32_t v) {
#if defined(WLIB_LITTLE_ENDIAN)
  *(uint32_t *)dest = v;
#elif defined(WLIB_BIG_ENDIAN)
  write2b(dest, (uint16_t)(v & 0xFFFF));
  write2b((byte *)dest + 2, (uint16_t)((v >> 16) & 0xFFFF));
#else
  wl_a(false);
#endif
}

static inline void write8b(void *dest, uint64_t v) {
#if defined(WLIB_LITTLE_ENDIAN)
  *(uint64_t *)dest = v;
#elif defined(WLIB_BIG_ENDIAN)
  write4b(dest, (uint32_t)(v & 0xFFFFFFFF));
  write4b((byte *)dest + 4, (uint32_t)((v >> 32) & 0xFFFFFFFF));
#else
  wl_a(false);
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
  wl_a(false);
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
  wl_a(false);
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
  wl_a(false);
#endif
}

/** Creates a 64-bit integer out of two 32-bit integers.
@param[in]      high            high-order 32 bits
@param[in]      low             low-order 32 bits
@return created integer */
uint64_t ut_ull_create(uint32_t high, uint32_t low) {
  return ((uint64_t)high) << 32 | low;
}

/** Rounds a 64-bit integer downward to a multiple of a power of 2.
@param[in]      n               number to be rounded
@param[in]      align_no        align by this number
@return rounded value */
static inline uint64_t ut_uint64_align_down(uint64_t n, unsigned long int align_no) {
  wl_ad(align_no > 0);
  wl_ad(calc_is_2pow(align_no));

  return (n & ~((uint64_t)align_no - 1));
}

/** Rounds uint64_t upward to a multiple of a power of 2.
@param[in]      n               number to be rounded
@param[in]      align_no        align by this number
@return rounded value */
static inline uint64_t ut_uint64_align_up(uint64_t n, unsigned long int align_no) {
  const uint64_t align_1 = (uint64_t)align_no - 1;

  wl_ad(align_no > 0);
  wl_ad(calc_is_2pow(align_no));

  return (n + align_1) & ~align_1;
}

/** The following function rounds up a pointer to the nearest aligned address.
@param[in]      ptr             pointer
@param[in]      align_no        align by this number
@return aligned pointer */
static inline void *ut_align(const void *ptr, unsigned long int align_no) {
  wl_ad(align_no > 0);
  wl_ad(((align_no - 1) & align_no) == 0);
  wl_ad(ptr);

  wl_ad(sizeof(void *) == sizeof(unsigned long int));

  return ((void *)((((unsigned long int)ptr) + align_no - 1) & ~(align_no - 1)));
}

/** The following function rounds down a pointer to the nearest aligned address.
@param[in]      ptr             pointer
@param[in]      align_no        align by this number
@return aligned pointer */
static inline void *ut_align_down(const void *ptr, unsigned long int align_no) {
  wl_ad(align_no > 0);
  wl_ad(((align_no - 1) & align_no) == 0);
  wl_ad(ptr);

  wl_ad(sizeof(void *) == sizeof(unsigned long int));

  return ((void *)((((unsigned long int)ptr)) & ~(align_no - 1)));
}

/** The following function computes the offset of a pointer from the nearest
aligned address.
@param[in]      ptr             pointer
@param[in]      align_no        align by this number
@return distance from aligned pointer */
static inline unsigned long int ut_align_offset(const void *ptr, unsigned long int align_no) {
  wl_ad(align_no > 0);
  wl_ad(((align_no - 1) & align_no) == 0);
  wl_ad(ptr);

  wl_ad(sizeof(void *) == sizeof(unsigned long int));

  return (((unsigned long int)ptr) & (align_no - 1));
}

/** Gets the nth bit of a unsigned long int.
@param[in]      a       unsigned long int
@param[in]      n       nth bit requested
@return true if nth bit is 1; 0th bit is defined to be the least significant */
static inline bool ut_bit_get_nth(unsigned long int a, unsigned long int n) {
  wl_ad(n < 8 * sizeof(unsigned long int));
  return (1 & (a >> n));
}

/** Sets the nth bit of a unsigned long int.
@param[in]      a       unsigned long int
@param[in]      n       nth bit requested
@param[in]      val     value for the bit to set
@return the unsigned long int with the bit set as requested */
static inline unsigned long int ut_bit_set_nth(unsigned long int a, unsigned long int n, bool val) {
  wl_ad(n < 8 * sizeof(unsigned long int));
  if (val) {
    return (((unsigned long int)1 << n) | a);
  } else {
    return (~((unsigned long int)1 << n) & a);
  }
}

/** The following function is used to store data in one byte.
@param[in]      b       pointer to byte where to store
@param[in]      n       unsigned long int integer to be stored, >= 0, < 256 */
static inline void mach_write_to_1(byte *b, unsigned long int n) {
  wl_ad(b);
  wl_ad((n | 0xFFUL) <= 0xFFUL);

  b[0] = (byte)n;
}

/** The following function is used to store data in two consecutive
bytes. We store the most significant byte to the lower address.
@param[in]      b       pointer to 2 bytes where to store
@param[in]      n       2 byte integer to be stored, >= 0, < 64k */
static inline void mach_write_to_2(byte *b, unsigned long int n) {
  wl_ad(b);
  wl_ad((n | 0xFFFFUL) <= 0xFFFFUL);

  b[0] = (byte)(n >> 8);
  b[1] = (byte)(n);
}

/** The following function is used to fetch data from one byte.
@param[in]      b       pointer to a byte to read
@return unsigned long int integer, >= 0, < 256 */
static inline uint8_t mach_read_from_1(const byte *b) {
  wl_ad(b);
  return ((uint8_t)(b[0]));
}

/** The following function is used to fetch data from 2 consecutive
bytes. The most significant byte is at the lowest address.
@param[in]      b       pointer to 2 bytes to read
@return 2-byte integer, >= 0, < 64k */
static inline uint16_t mach_read_from_2(const byte *b) {
  return (((unsigned long int)(b[0]) << 8) | (unsigned long int)(b[1]));
}

/** The following function is used to convert a 16-bit data item
 to the canonical format, for fast bytewise equality test
 against memory.
 @return 16-bit integer in canonical format */
static inline uint16_t mach_encode_2(
    unsigned long int n) /*!< in: integer in machine-dependent format */
{
  uint16_t ret;
  wl_ad(2 == sizeof ret);
  mach_write_to_2((byte *)&ret, n);
  return (ret);
}
/** The following function is used to convert a 16-bit data item
 from the canonical format, for fast bytewise equality test
 against memory.
 @return integer in machine-dependent format */
static inline unsigned long int mach_decode_2(
    uint16_t n) /*!< in: 16-bit integer in canonical format */
{
  wl_ad(2 == sizeof n);
  return (mach_read_from_2((const byte *)&n));
}

/** The following function is used to store data in 3 consecutive
bytes. We store the most significant byte to the lowest address.
@param[in]      b       pointer to 3 bytes where to store
@param[in]      n       3 byte integer to be stored */
static inline void mach_write_to_3(byte *b, unsigned long int n) {
  wl_ad(b);
  wl_ad((n | 0xFFFFFFUL) <= 0xFFFFFFUL);

  b[0] = (byte)(n >> 16);
  b[1] = (byte)(n >> 8);
  b[2] = (byte)(n);
}

/** The following function is used to fetch data from 3 consecutive
bytes. The most significant byte is at the lowest address.
@param[in]      b       pointer to 3 bytes to read
@return uint32_t integer */
static inline uint32_t mach_read_from_3(const byte *b) {
  wl_ad(b);
  return (((uint32_t)(b[0]) << 16) |
          ((uint32_t)(b[1]) << 8) | (uint32_t)(b[2]));
}

/** The following function is used to store data in 4 consecutive
bytes. We store the most significant byte to the lowest address.
@param[in]      b       pointer to 4 bytes where to store
@param[in]      n       4 byte integer to be stored */
static inline void mach_write_to_4(byte *b, unsigned long int n) {
  wl_ad(b);

  b[0] = (byte)(n >> 24);
  b[1] = (byte)(n >> 16);
  b[2] = (byte)(n >> 8);
  b[3] = (byte)(n);
}

/** The following function is used to fetch data from 4 consecutive
bytes. The most significant byte is at the lowest address.
@param[in]      b       pointer to 4 bytes to read
@return 32 bit integer */
static inline uint32_t mach_read_from_4(const byte *b) {
  wl_ad(b);
  return (((uint32_t)(b[0]) << 24) |
          ((uint32_t)(b[1]) << 16) |
          ((uint32_t)(b[2]) << 8) | (uint32_t)(b[3]));
}

/** Writes a unsigned long int in a compressed form where the first byte codes the
length of the stored unsigned long int. We look at the most significant bits of
the byte. If the most significant bit is zero, it means 1-byte storage,
else if the 2nd bit is 0, it means 2-byte storage, else if 3rd is 0,
it means 3-byte storage, else if 4th is 0, it means 4-byte storage,
else the storage is 5-byte.
@param[in]      b       pointer to memory where to store
@param[in]      n       unsigned long int integer (< 2^32) to be stored
@return compressed size in bytes */
static inline unsigned long int mach_write_compressed(byte *b, unsigned long int n) {
  wl_ad(b);

  if (n < 0x80) {
    /* 0nnnnnnn (7 bits) */
    mach_write_to_1(b, n);
    return (1);
  } else if (n < 0x4000) {
    /* 10nnnnnn nnnnnnnn (14 bits) */
    mach_write_to_2(b, n | 0x8000);
    return (2);
  } else if (n < 0x200000) {
    /* 110nnnnn nnnnnnnn nnnnnnnn (21 bits) */
    mach_write_to_3(b, n | 0xC00000);
    return (3);
  } else if (n < 0x10000000) {
    /* 1110nnnn nnnnnnnn nnnnnnnn nnnnnnnn (28 bits) */
    mach_write_to_4(b, n | 0xE0000000);
    return (4);
  } else if (n >= 0xFFFFFC00) {
    /* 111110nn nnnnnnnn (10 bits) (extended) */
    mach_write_to_2(b, (n & 0x3FF) | 0xF800);
    return (2);
  } else if (n >= 0xFFFE0000) {
    /* 1111110n nnnnnnnn nnnnnnnn (17 bits) (extended) */
    mach_write_to_3(b, (n & 0x1FFFF) | 0xFC0000);
    return (3);
  } else if (n >= 0xFF000000) {
    /* 11111110 nnnnnnnn nnnnnnnn nnnnnnnn (24 bits) (extended) */
    mach_write_to_4(b, (n & 0xFFFFFF) | 0xFE000000);
    return (4);
  } else {
    /* 11110000 nnnnnnnn nnnnnnnn nnnnnnnn nnnnnnnn (32 bits) */
    mach_write_to_1(b, 0xF0);
    mach_write_to_4(b + 1, n);
    return (5);
  }
}

/** Return the size of an unsigned long int when written in the compressed form.
@param[in]      n       uint32_t integer (< 2^32) to be stored
@return compressed size in bytes */
static inline uint32_t mach_get_compressed_size(unsigned long int n) {
  if (n < 0x80) {
    /* 0nnnnnnn (7 bits) */
    return (1);
  } else if (n < 0x4000) {
    /* 10nnnnnn nnnnnnnn (14 bits) */
    return (2);
  } else if (n < 0x200000) {
    /* 110nnnnn nnnnnnnn nnnnnnnn (21 bits) */
    return (3);
  } else if (n < 0x10000000) {
    /* 1110nnnn nnnnnnnn nnnnnnnn nnnnnnnn (28 bits) */
    return (4);
  } else if (n >= 0xFFFFFC00) {
    /* 111110nn nnnnnnnn (10 bits) (extended) */
    return (2);
  } else if (n >= 0xFFFE0000) {
    /* 1111110n nnnnnnnn nnnnnnnn (17 bits) (extended) */
    return (3);
  } else if (n >= 0xFF000000) {
    /* 11111110 nnnnnnnn nnnnnnnn nnnnnnnn (24 bits) (extended) */
    return (4);
  } else {
    /* 11110000 nnnnnnnn nnnnnnnn nnnnnnnn nnnnnnnn (32 bits) */
    return (5);
  }
}

/** Read a unsigned long int in a compressed form.
@param[in]      b       pointer to memory from where to read
@return read integer (< 2^32) */
static inline unsigned long int mach_read_compressed(const byte *b) {
  unsigned long int val;

  wl_ad(b);

  val = mach_read_from_1(b);

  if (val < 0x80) {
    /* 0nnnnnnn (7 bits) */
  } else if (val < 0xC0) {
    /* 10nnnnnn nnnnnnnn (14 bits) */
    val = mach_read_from_2(b) & 0x3FFF;
    wl_ad(val > 0x7F);
  } else if (val < 0xE0) {
    /* 110nnnnn nnnnnnnn nnnnnnnn (21 bits) */
    val = mach_read_from_3(b) & 0x1FFFFF;
    wl_ad(val > 0x3FFF);
  } else if (val < 0xF0) {
    /* 1110nnnn nnnnnnnn nnnnnnnn nnnnnnnn (28 bits) */
    val = mach_read_from_4(b) & 0xFFFFFFF;
    wl_ad(val > 0x1FFFFF);
  } else if (val < 0xF8) {
    /* 11110000 nnnnnnnn nnnnnnnn nnnnnnnn nnnnnnnn (32 bits) */
    wl_ad(val == 0xF0);
    val = mach_read_from_4(b + 1);
    /* this can treat not-extended format also. */
    wl_ad(val > 0xFFFFFFF);
  } else if (val < 0xFC) {
    /* 111110nn nnnnnnnn (10 bits) (extended) */
    val = (mach_read_from_2(b) & 0x3FF) | 0xFFFFFC00;
  } else if (val < 0xFE) {
    /* 1111110n nnnnnnnn nnnnnnnn (17 bits) (extended) */
    val = (mach_read_from_3(b) & 0x1FFFF) | 0xFFFE0000;
    wl_ad(val < 0xFFFFFC00);
  } else {
    /* 11111110 nnnnnnnn nnnnnnnn nnnnnnnn (24 bits) (extended) */
    wl_ad(val == 0xFE);
    val = mach_read_from_3(b + 1) | 0xFF000000;
    wl_ad(val < 0xFFFE0000);
  }

  return (val);
}

/** Read a 32-bit integer in a compressed form.
@param[in,out]  b       pointer to memory where to read;
advanced by the number of bytes consumed
@return unsigned value */
static inline uint32_t mach_read_next_compressed(const byte **b) {
  unsigned long int val = mach_read_from_1(*b);

  if (val < 0x80) {
    /* 0nnnnnnn (7 bits) */
    ++*b;
  } else if (val < 0xC0) {
    /* 10nnnnnn nnnnnnnn (14 bits) */
    val = mach_read_from_2(*b) & 0x3FFF;
    wl_ad(val > 0x7F);
    *b += 2;
  } else if (val < 0xE0) {
    /* 110nnnnn nnnnnnnn nnnnnnnn (21 bits) */
    val = mach_read_from_3(*b) & 0x1FFFFF;
    wl_ad(val > 0x3FFF);
    *b += 3;
  } else if (val < 0xF0) {
    /* 1110nnnn nnnnnnnn nnnnnnnn nnnnnnnn (28 bits) */
    val = mach_read_from_4(*b) & 0xFFFFFFF;
    wl_ad(val > 0x1FFFFF);
    *b += 4;
  } else if (val < 0xF8) {
    /* 11110000 nnnnnnnn nnnnnnnn nnnnnnnn nnnnnnnn (32 bits) */
    wl_ad(val == 0xF0);
    val = mach_read_from_4(*b + 1);
    /* this can treat not-extended format also. */
    wl_ad(val > 0xFFFFFFF);
    *b += 5;
  } else if (val < 0xFC) {
    /* 111110nn nnnnnnnn (10 bits) (extended) */
    val = (mach_read_from_2(*b) & 0x3FF) | 0xFFFFFC00;
    *b += 2;
  } else if (val < 0xFE) {
    /* 1111110n nnnnnnnn nnnnnnnn (17 bits) (extended) */
    val = (mach_read_from_3(*b) & 0x1FFFF) | 0xFFFE0000;
    wl_ad(val < 0xFFFFFC00);
    *b += 3;
  } else {
    /* 11111110 nnnnnnnn nnnnnnnn nnnnnnnn (24 bits) (extended) */
    wl_ad(val == 0xFE);
    val = mach_read_from_3(*b + 1) | 0xFF000000;
    wl_ad(val < 0xFFFE0000);
    *b += 4;
  }

  return ((uint32_t)(val));
}

/** The following function is used to store data in 8 consecutive
bytes. We store the most significant byte to the lowest address.
@param[in]      b       pointer to 8 bytes where to store
@param[in]      n       64-bit integer (< 2^64) to be stored */
static inline void mach_write_to_8(void *b, uint64_t n) {
  wl_ad(b);

  mach_write_to_4((byte *)(b), (unsigned long int)(n >> 32));
  mach_write_to_4((byte *)(b) + 4, (unsigned long int)n);
}

/** The following function is used to fetch data from 8 consecutive
bytes. The most significant byte is at the lowest address.
@param[in]      b       pointer to 8 bytes from where read
@return 64-bit integer */
static inline uint64_t mach_read_from_8(const byte *b) {
  uint64_t u64;

  u64 = mach_read_from_4(b);
  u64 <<= 32;
  u64 |= mach_read_from_4(b + 4);

  return (u64);
}

/** The following function is used to store data in 7 consecutive
bytes. We store the most significant byte to the lowest address.
@param[in]      b       pointer to 7 bytes where to store
@param[in]      n       56-bit integer */
static inline void mach_write_to_7(byte *b, uint64_t n) {
  wl_ad(b);

  mach_write_to_3(b, (unsigned long int)(n >> 32));
  mach_write_to_4(b + 3, (unsigned long int)n);
}

/** The following function is used to fetch data from 7 consecutive
bytes. The most significant byte is at the lowest address.
@param[in]      b       pointer to 7 bytes to read
@return 56-bit integer */
static inline uint64_t mach_read_from_7(const byte *b) {
  wl_ad(b);

  return (ut_ull_create(mach_read_from_3(b), mach_read_from_4(b + 3)));
}

/** The following function is used to store data in 6 consecutive
bytes. We store the most significant byte to the lowest address.
@param[in]      b       pointer to 6 bytes where to store
@param[in]      n       48-bit integer to write */
static inline void mach_write_to_6(byte *b, uint64_t n) {
  wl_ad(b);

  mach_write_to_2(b, (unsigned long int)(n >> 32));
  mach_write_to_4(b + 2, (unsigned long int)n);
}

/** The following function is used to fetch data from 6 consecutive
bytes. The most significant byte is at the lowest address.
@param[in]      b       pointer to 6 bytes to read
@return 48-bit integer */
static inline uint64_t mach_read_from_6(const byte *b) {
  wl_ad(b);

  return (ut_ull_create(mach_read_from_2(b), mach_read_from_4(b + 2)));
}

/** Writes a 64-bit integer in a compressed form (5..9 bytes).
 @return size in bytes */
static inline unsigned long int mach_u64_write_compressed(
    byte *b,    /*!< in: pointer to memory where to store */
    uint64_t n) /*!< in: 64-bit integer to be stored */
{
  unsigned long int size;

  wl_ad(b);

  size = mach_write_compressed(b, (unsigned long int)(n >> 32));
  mach_write_to_4(b + size, (unsigned long int)n);

  return (size + 4);
}

/** Read a 64-bit integer in a compressed form.
@param[in,out]  b       pointer to memory where to read;
advanced by the number of bytes consumed
@return unsigned value */
static inline uint64_t mach_u64_read_next_compressed(const byte **b) {
  uint64_t val;

  val = mach_read_next_compressed(b);
  val <<= 32;
  val |= mach_read_from_4(*b);
  *b += 4;
  return (val);
}

/** Writes a 64-bit integer in a compressed form (1..11 bytes).
 @return size in bytes */
static inline unsigned long int mach_u64_write_much_compressed(
    byte *b,    /*!< in: pointer to memory where to store */
    uint64_t n) /*!< in: 64-bit integer to be stored */
{
  unsigned long int size;

  wl_ad(b);

  if (!(n >> 32)) {
    return (mach_write_compressed(b, (unsigned long int)n));
  }

  *b = (byte)0xFF;
  size = 1 + mach_write_compressed(b + 1, (unsigned long int)(n >> 32));

  size += mach_write_compressed(b + size, (unsigned long int)n & 0xFFFFFFFF);

  return (size);
}

/** Reads a 64-bit integer in a compressed form.
 @return the value read
 @see mach_parse_u64_much_compressed() */
static inline uint64_t mach_u64_read_much_compressed(
    const byte *b) /*!< in: pointer to memory from where to read */
{
  uint64_t n;

  if (*b != 0xFF) {
    return (mach_read_compressed(b));
  }

  b++;
  n = mach_read_next_compressed(&b);
  n <<= 32;
  n |= mach_read_compressed(b);

  return (n);
}

/** Read a 64-bit integer in a compressed form.
@param[in,out]  b       pointer to memory where to read;
advanced by the number of bytes consumed
@return unsigned value */
static inline uint64_t mach_read_next_much_compressed(const byte **b) {
  uint64_t val = mach_read_from_1(*b);

  if (val < 0x80) {
    /* 0nnnnnnn (7 bits) */
    ++*b;
  } else if (val < 0xC0) {
    /* 10nnnnnn nnnnnnnn (14 bits) */
    val = mach_read_from_2(*b) & 0x3FFF;
    wl_ad(val > 0x7F);
    *b += 2;
  } else if (val < 0xE0) {
    /* 110nnnnn nnnnnnnn nnnnnnnn (21 bits) */
    val = mach_read_from_3(*b) & 0x1FFFFF;
    wl_ad(val > 0x3FFF);
    *b += 3;
  } else if (val < 0xF0) {
    /* 1110nnnn nnnnnnnn nnnnnnnn nnnnnnnn (28 bits) */
    val = mach_read_from_4(*b) & 0xFFFFFFF;
    wl_ad(val > 0x1FFFFF);
    *b += 4;
  } else if (val < 0xF8) {
    /* 11110000 nnnnnnnn nnnnnnnn nnnnnnnn nnnnnnnn (32 bits) */
    wl_ad(val == 0xF0);
    val = mach_read_from_4(*b + 1);
    /* this can treat not-extended format also. */
    wl_ad(val > 0xFFFFFFF);
    *b += 5;
  } else if (val < 0xFC) {
    /* 111110nn nnnnnnnn (10 bits) (extended) */
    val = (mach_read_from_2(*b) & 0x3FF) | 0xFFFFFC00;
    *b += 2;
  } else if (val < 0xFE) {
    /* 1111110n nnnnnnnn nnnnnnnn (17 bits) (extended) */
    val = (mach_read_from_3(*b) & 0x1FFFF) | 0xFFFE0000;
    wl_ad(val < 0xFFFFFC00);
    *b += 3;
  } else if (val == 0xFE) {
    /* 11111110 nnnnnnnn nnnnnnnn nnnnnnnn (24 bits) (extended) */
    wl_ad(val == 0xFE);
    val = mach_read_from_3(*b + 1) | 0xFF000000;
    wl_ad(val < 0xFFFE0000);
    *b += 4;
  } else {
    /* 11111111 followed by up to 80 bits */
    wl_ad(val == 0xFF);
    ++*b;
    val = mach_read_next_compressed(b);
    wl_ad(val > 0);
    val <<= 32;
    val |= mach_read_next_compressed(b);
  }

  return (val);
}

/** Read a 64-bit integer in a compressed form.
@param[in,out]  ptr     pointer to memory where to read;
advanced by the number of bytes consumed, or set NULL if out of space
@param[in]      end_ptr end of the buffer
@return unsigned value */
static inline uint64_t mach_u64_parse_compressed(const byte **ptr,
                                                 const byte *end_ptr) {
  uint64_t val = 0;

  if (end_ptr < *ptr + 5) {
    *ptr = NULL;
    return (val);
  }

  val = mach_read_next_compressed(ptr);

  if (end_ptr < *ptr + 4) {
    *ptr = NULL;
    return (val);
  }

  val <<= 32;
  val |= mach_read_from_4(*ptr);
  *ptr += 4;

  return (val);
}

/** Reads a double. It is stored in a little-endian format.
 @return double read */
static inline double mach_double_read(
    const byte *b) /*!< in: pointer to memory from where to read */
{
  double d;
  unsigned long int i;
  byte *ptr;

  ptr = (byte *)&d;

  for (i = 0; i < sizeof(double); i++) {
#ifdef WORDS_BIGENDIAN
    ptr[sizeof(double) - i - 1] = b[i];
#else
    ptr[i] = b[i];
#endif
  }

  return (d);
}

/** Writes a double. It is stored in a little-endian format. */
static inline void mach_double_write(
    byte *b,  /*!< in: pointer to memory where to write */
    double d) /*!< in: double */
{
  unsigned long int i;
  byte *ptr;

  ptr = (byte *)&d;

  for (i = 0; i < sizeof(double); i++) {
#ifdef WORDS_BIGENDIAN
    b[i] = ptr[sizeof(double) - i - 1];
#else
    b[i] = ptr[i];
#endif
  }
}

/** Reads a float. It is stored in a little-endian format.
 @return float read */
static inline float mach_float_read(
    const byte *b) /*!< in: pointer to memory from where to read */
{
  float d;
  unsigned long int i;
  byte *ptr;

  ptr = (byte *)&d;

  for (i = 0; i < sizeof(float); i++) {
#ifdef WORDS_BIGENDIAN
    ptr[sizeof(float) - i - 1] = b[i];
#else
    ptr[i] = b[i];
#endif
  }

  return (d);
}

/** Writes a float. It is stored in a little-endian format. */
static inline void mach_float_write(
    byte *b, /*!< in: pointer to memory where to write */
    float d) /*!< in: float */
{
  unsigned long int i;
  byte *ptr;

  ptr = (byte *)&d;

  for (i = 0; i < sizeof(float); i++) {
#ifdef WORDS_BIGENDIAN
    b[i] = ptr[sizeof(float) - i - 1];
#else
    b[i] = ptr[i];
#endif
  }
}

static inline unsigned long int mach_read_from_n_little_endian(const byte *buf,
                                                   unsigned long int buf_size) {
  unsigned long int n = 0;
  const byte *ptr;

  wl_ad(buf_size > 0);

  ptr = buf + buf_size;

  for (;;) {
    ptr--;

    n = n << 8;

    n += (unsigned long int)(*ptr);

    if (ptr == buf) {
      break;
    }
  }

  return (n);
}

/** Writes a unsigned long int in the little-endian format. */
static inline void mach_write_to_n_little_endian(
    byte *dest,      /*!< in: where to write */
    unsigned long int dest_size, /*!< in: into how many bytes to write */
    unsigned long int n)         /*!< in: unsigned long int to write */
{
  byte *end;

  wl_ad(dest_size <= sizeof(unsigned long int));
  wl_ad(dest_size > 0);

  end = dest + dest_size;

  for (;;) {
    *dest = (byte)(n & 0xFF);

    n = n >> 8;

    dest++;

    if (dest == end) {
      break;
    }
  }

  wl_ad(n == 0);
}

/** Reads a unsigned long int stored in the little-endian format.
 @return unsigned long int */
static inline unsigned long int mach_read_from_2_little_endian(
    const byte *buf) /*!< in: from where to read */
{
  return ((unsigned long int)(buf[0]) | ((unsigned long int)(buf[1]) << 8));
}

/** Writes a unsigned long int in the little-endian format. */
static inline void mach_write_to_2_little_endian(
    byte *dest, /*!< in: where to write */
    unsigned long int n)    /*!< in: unsigned long int to write */
{
  wl_ad(n < 256 * 256);

  *dest = (byte)(n & 0xFFUL);

  n = n >> 8;
  dest++;

  *dest = (byte)(n & 0xFFUL);
}

/** Convert integral type from storage byte order (big endian) to
 host byte order.
 @return integer value */
static inline uint64_t mach_read_int_type(
    const byte *src,    /*!< in: where to read from */
    unsigned long int len,          /*!< in: length of src */
    bool unsigned_type) /*!< in: signed or unsigned flag */
{
  /* XXX this can be optimized on big-endian machines */

  uintmax_t ret;
  unsigned int i;

  if (unsigned_type || (src[0] & 0x80)) {
    ret = 0x0000000000000000ULL;
  } else {
    ret = 0xFFFFFFFFFFFFFF00ULL;
  }

  if (unsigned_type) {
    ret |= src[0];
  } else {
    ret |= src[0] ^ 0x80;
  }

  for (i = 1; i < len; i++) {
    ret <<= 8;
    ret |= src[i];
  }

  return (ret);
}

/** Swap byte ordering. */
static inline void mach_swap_byte_order(
    byte *dest,       /*!< out: where to write */
    const byte *from, /*!< in: where to read from */
    unsigned long int len)        /*!< in: length of src */
{
  wl_ad(len > 0);
  wl_ad(len <= 8);

  dest += len;

  switch (len & 0x7) {
    case 0:
      *--dest = *from++;
      [[fallthrough]];
    case 7:
      *--dest = *from++;
      [[fallthrough]];
    case 6:
      *--dest = *from++;
      [[fallthrough]];
    case 5:
      *--dest = *from++;
      [[fallthrough]];
    case 4:
      *--dest = *from++;
      [[fallthrough]];
    case 3:
      *--dest = *from++;
      [[fallthrough]];
    case 2:
      *--dest = *from++;
      [[fallthrough]];
    case 1:
      *--dest = *from;
  }
}

/*************************************************************
Convert integral type from host byte order (big-endian) storage
byte order. */
static inline void mach_write_int_type(
    byte *dest,      /*!< in: where to write */
    const byte *src, /*!< in: where to read from */
    unsigned long int len,       /*!< in: length of src */
    bool usign)      /*!< in: signed or unsigned flag */
{
  wl_ad(len >= 1 && len <= 8);

#ifdef WORDS_BIGENDIAN
  memcpy(dest, src, len);
#else
  mach_swap_byte_order(dest, src, len);
#endif /* WORDS_BIGENDIAN */

  if (!usign) {
    *dest ^= 0x80;
  }
}

/*************************************************************
Convert a ulonglong integer from host byte order to (big-endian)
storage byte order. */
static inline void mach_write_ulonglong(
    byte *dest,    /*!< in: where to write */
    unsigned long long src, /*!< in: where to read from */
    unsigned long int len,     /*!< in: length of dest */
    bool usign)    /*!< in: signed or unsigned flag */
{
  byte *ptr = (byte *)(&src);

  wl_ad(len <= sizeof(unsigned long long));

#ifdef WORDS_BIGENDIAN
  memcpy(dest, ptr + (sizeof(src) - len), len);
#else
  mach_swap_byte_order(dest, (byte *)(ptr), len);
#endif /* WORDS_BIGENDIAN */

  if (!usign) {
    *dest ^= 0x80;
  }
}

/** Read a 32-bit integer in a compressed form.
@param[in,out]  ptr     pointer to memory from where to read;
advanced by the number of bytes consumed, or set NULL if out of space
@param[in]      end_ptr end of the buffer
@return unsigned value */
static inline uint32_t mach_parse_compressed(const byte **ptr, const byte *end_ptr) {
  unsigned long int val;

  if (*ptr >= end_ptr) {
    *ptr = NULL;
    return (0);
  }

  val = mach_read_from_1(*ptr);

  if (val < 0x80) {
    /* 0nnnnnnn (7 bits) */
    ++*ptr;
    return ((uint32_t)(val));
  }

  /* Workaround GCC bug
  https://gcc.gnu.org/bugzilla/show_bug.cgi?id=77673:
  the compiler moves mach_read_from_4 right to the beginning of the
  function, causing and out-of-bounds read if we are reading a short
  integer close to the end of buffer. */
#if defined(__GNUC__) && (__GNUC__ >= 5) && !defined(__clang__)
#define DEPLOY_FENCE
#endif

#ifdef DEPLOY_FENCE
  __atomic_thread_fence(__ATOMIC_ACQUIRE);
#endif

  if (val < 0xC0) {
    /* 10nnnnnn nnnnnnnn (14 bits) */
    if (end_ptr >= *ptr + 2) {
      val = mach_read_from_2(*ptr) & 0x3FFF;
      wl_ad(val > 0x7F);
      *ptr += 2;
      return ((uint32_t)(val));
    }
    *ptr = NULL;
    return (0);
  }

#ifdef DEPLOY_FENCE
  __atomic_thread_fence(__ATOMIC_ACQUIRE);
#endif

  if (val < 0xE0) {
    /* 110nnnnn nnnnnnnn nnnnnnnn (21 bits) */
    if (end_ptr >= *ptr + 3) {
      val = mach_read_from_3(*ptr) & 0x1FFFFF;
      wl_ad(val > 0x3FFF);
      *ptr += 3;
      return ((uint32_t)(val));
    }
    *ptr = NULL;
    return (0);
  }

#ifdef DEPLOY_FENCE
  __atomic_thread_fence(__ATOMIC_ACQUIRE);
#endif

  if (val < 0xF0) {
    /* 1110nnnn nnnnnnnn nnnnnnnn nnnnnnnn (28 bits) */
    if (end_ptr >= *ptr + 4) {
      val = mach_read_from_4(*ptr) & 0xFFFFFFF;
      wl_ad(val > 0x1FFFFF);
      *ptr += 4;
      return ((uint32_t)(val));
    }
    *ptr = NULL;
    return (0);
  }

#ifdef DEPLOY_FENCE
  __atomic_thread_fence(__ATOMIC_ACQUIRE);
#endif

  if (val < 0xF8) {
    wl_ad(val == 0xF0);

    /* 11110000 nnnnnnnn nnnnnnnn nnnnnnnn nnnnnnnn (32 bits) */
    if (end_ptr >= *ptr + 5) {
      val = mach_read_from_4(*ptr + 1);
      wl_ad(val > 0xFFFFFFF);
      *ptr += 5;
      return ((uint32_t)(val));
    }

    *ptr = NULL;
    return (0);
  }

#ifdef DEPLOY_FENCE
  __atomic_thread_fence(__ATOMIC_ACQUIRE);
#endif

  if (val < 0xFC) {
    /* 111110nn nnnnnnnn (10 bits) (extended) */
    if (end_ptr >= *ptr + 2) {
      val = (mach_read_from_2(*ptr) & 0x3FF) | 0xFFFFFC00;
      *ptr += 2;
      return ((uint32_t)(val));
    }
    *ptr = NULL;
    return (0);
  }

#ifdef DEPLOY_FENCE
  __atomic_thread_fence(__ATOMIC_ACQUIRE);
#endif

  if (val < 0xFE) {
    /* 1111110n nnnnnnnn nnnnnnnn (17 bits) (extended) */
    if (end_ptr >= *ptr + 3) {
      val = (mach_read_from_3(*ptr) & 0x1FFFF) | 0xFFFE0000;
      wl_ad(val < 0xFFFFFC00);
      *ptr += 3;
      return ((uint32_t)(val));
    }
    *ptr = NULL;
    return (0);
  }

#ifdef DEPLOY_FENCE
  __atomic_thread_fence(__ATOMIC_ACQUIRE);
#endif

#undef DEPLOY_FENCE

  wl_ad(val == 0xFE);

  /* 11111110 nnnnnnnn nnnnnnnn nnnnnnnn (24 bits) (extended) */
  if (end_ptr >= *ptr + 4) {
    val = mach_read_from_3(*ptr + 1) | 0xFF000000;
    wl_ad(val < 0xFFFE0000);
    *ptr += 4;
    return ((uint32_t)(val));
  }

  *ptr = NULL;
  return (0);
}

/** Read a 64-bit integer in a much compressed form.
@param[in,out]  ptr     pointer to memory from where to read,
advanced by the number of bytes consumed, or set NULL if out of space
@param[in]      end_ptr end of the buffer
@return unsigned 64-bit integer */
uint64_t mach_parse_u64_much_compressed(const byte **ptr, const byte *end_ptr) {
  unsigned long int val;

  if (*ptr >= end_ptr) {
    *ptr = NULL;
    return (0);
  }

  val = mach_read_from_1(*ptr);

  if (val != 0xFF) {
    return (mach_parse_compressed(ptr, end_ptr));
  }

  ++*ptr;

  uint64_t n = mach_parse_compressed(ptr, end_ptr);
  if (*ptr == NULL) {
    return (0);
  }

  n <<= 32;

  n |= mach_parse_compressed(ptr, end_ptr);
  if (*ptr == NULL) {
    return (0);
  }

  return (n);
}

#endif