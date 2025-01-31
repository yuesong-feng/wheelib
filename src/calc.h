#ifndef CALC_H
#define CALC_H
#include "byte.h"
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// Determines if a number is zero or a power of two.
#define calc_is_2pow(n) (!((n) & ((n) - 1)))

// Calculates fast the remainder of n/m when m is a power of two.
#define calc_2pow_remainder(n, m) ((n) & ((m) - 1))

// Calculates the biggest multiple of m that is not bigger than n
#define calc_2pow_round(n, m) ((n) & ~((m) - 1))

// Align a number down to a multiple of a power of two.
#define calc_align_down(n, m) calc_2pow_round(n, m)

// Calculates the smallest multiple of m that is not smaller than n
#define calc_align(n, m) (((n) + ((m) - 1)) & ~((m) - 1))

// Determine how many bytes (groups of 8 bits) are needed to store the given number of bits.
#define BITS_IN_BYTES(b) (((b) + 7) / 8)

// Calculates fast the 2-logarithm of a number, rounded upward to an integer.
size_t calc_2_log(size_t n);

// Calculates 2 to power n.
size_t calc_2_exp(size_t n);

// Calculates fast the number rounded up to the nearest power of 2.
size_t calc_2_power_up(size_t n);

size_t calc_find_prime(size_t n);

size_t calc_fold_pair(size_t n1, size_t n2);

size_t calc_fold_uint64(uint64_t d);

size_t calc_fold_string(const char *str);

size_t calc_fold_binary(const byte *str, size_t len);

uint64_t uint64_create(size_t high, size_t low);

bool calc_add_s8_overflow(int8_t a, int8_t b, int8_t *result);
bool calc_sub_s8_overflow(int8_t a, int8_t b, int8_t *result);
bool calc_mul_s8_overflow(int8_t a, int8_t b, int8_t *result);

bool calc_add_s16_overflow(int16_t a, int16_t b, int16_t *result);
bool calc_sub_s16_overflow(int16_t a, int16_t b, int16_t *result);
bool calc_mul_s16_overflow(int16_t a, int16_t b, int16_t *result);

bool calc_add_s32_overflow(int32_t a, int32_t b, int32_t *result);
bool calc_sub_s32_overflow(int32_t a, int32_t b, int32_t *result);
bool calc_mul_s32_overflow(int32_t a, int32_t b, int32_t *result);

bool calc_add_s64_overflow(int64_t a, int64_t b, int64_t *result);
bool calc_sub_s64_overflow(int64_t a, int64_t b, int64_t *result);
bool calc_mul_s64_overflow(int64_t a, int64_t b, int64_t *result);

bool calc_add_u8_overflow(uint8_t a, uint8_t b, uint8_t *result);
bool calc_sub_u8_overflow(uint8_t a, uint8_t b, uint8_t *result);
bool calc_mul_u8_overflow(uint8_t a, uint8_t b, uint8_t *result);

bool calc_add_u16_overflow(uint16_t a, uint16_t b, uint16_t *result);
bool calc_sub_u16_overflow(uint16_t a, uint16_t b, uint16_t *result);
bool calc_mul_u16_overflow(uint16_t a, uint16_t b, uint16_t *result);

bool calc_add_u32_overflow(uint32_t a, uint32_t b, uint32_t *result);
bool calc_sub_u32_overflow(uint32_t a, uint32_t b, uint32_t *result);
bool calc_mul_u32_overflow(uint32_t a, uint32_t b, uint32_t *result);

bool calc_add_u64_overflow(uint64_t a, uint64_t b, uint64_t *result);
bool calc_sub_u64_overflow(uint64_t a, uint64_t b, uint64_t *result);
bool calc_mul_u64_overflow(uint64_t a, uint64_t b, uint64_t *result);

#endif