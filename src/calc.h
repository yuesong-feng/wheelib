#ifndef CALC_H
#define CALC_H
#include "byte.h"
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

#endif