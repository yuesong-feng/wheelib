#include "calc.h"
#include <assert.h>

#define HASH_RANDOM_MASK 1463735687
#define HASH_RANDOM_MASK2 1653893711
#define UINT32_MASK 0xFFFFFFFF

size_t calc_2_log(size_t n) {
  size_t res = 0;
  assert(n > 0);
  n = n - 1;
  for (;;) {
    n = n / 2;
    if (n == 0)
      break;
    res++;
  }
  return res + 1;
}

size_t calc_2_exp(size_t n) {
  return (size_t)1 << n;
}

size_t calc_2_power_up(size_t n) {
  size_t res = 1;
  assert(n > 0);
  while (res < n) {
    res = res * 2;
  }
  return res;
}

#define RANDOM_1 1.0412321
#define RANDOM_2 1.1131347
#define RANDOM_3 1.0132677
size_t calc_find_prime(size_t n) {
  size_t pow2;
  size_t i;

  n += 100;

  pow2 = 1;
  while (pow2 * 2 < n) {
    pow2 = 2 * pow2;
  }

  if ((double)n < 1.05 * (double)pow2) {
    n = (size_t)((double)n * RANDOM_1);
  }

  pow2 = 2 * pow2;

  if ((double)n > 0.95 * (double)pow2) {
    n = (size_t)((double)n * RANDOM_2);
  }

  if (n > pow2 - 20) {
    n += 30;
  }

  /* Now we have n far enough from powers of 2. To make
  n more random (especially, if it was not near
  a power of 2), we then multiply it by a random number. */

  n = (size_t)((double)n * RANDOM_3);

  for (;; n++) {
    i = 2;
    while (i * i <= n) {
      if (n % i == 0) {
        goto next_n;
      }
      i++;
    }

    /* Found a prime */
    break;
  next_n:;
  }

  return (n);
}

size_t calc_fold_pair(size_t n1, size_t n2) {
  return (((((n1 ^ n2 ^ HASH_RANDOM_MASK2) << 8) + n1) ^ HASH_RANDOM_MASK) + n2);
}

size_t calc_fold_uint64(uint64_t d) {
  return (calc_fold_pair((size_t)d & UINT32_MASK, (size_t)(d >> 32)));
}

size_t calc_fold_string(const char *str) {
  size_t fold = 0;
  assert(str);
  while (*str != '\0') {
    fold += calc_fold_pair(fold, (size_t)(*str));
    str++;
  }
  return fold;
}

size_t calc_fold_binary(const byte *str, size_t len) {
  size_t fold = 0;
  const byte *str_end = str + (len & 0xFFFFFFF8);

  assert(str || !len);

  while (str < str_end) {
    fold = calc_fold_pair(fold, (size_t)(*str++));
    fold = calc_fold_pair(fold, (size_t)(*str++));
    fold = calc_fold_pair(fold, (size_t)(*str++));
    fold = calc_fold_pair(fold, (size_t)(*str++));
    fold = calc_fold_pair(fold, (size_t)(*str++));
    fold = calc_fold_pair(fold, (size_t)(*str++));
    fold = calc_fold_pair(fold, (size_t)(*str++));
    fold = calc_fold_pair(fold, (size_t)(*str++));
  }

  switch (len & 0x7) {
  case 7:
    fold = calc_fold_pair(fold, (size_t)(*str++));
  case 6:
    fold = calc_fold_pair(fold, (size_t)(*str++));
  case 5:
    fold = calc_fold_pair(fold, (size_t)(*str++));
  case 4:
    fold = calc_fold_pair(fold, (size_t)(*str++));
  case 3:
    fold = calc_fold_pair(fold, (size_t)(*str++));
  case 2:
    fold = calc_fold_pair(fold, (size_t)(*str++));
  case 1:
    fold = calc_fold_pair(fold, (size_t)(*str++));
  }

  return (fold);
}

uint64_t uint64_create(size_t high, size_t low) {
  assert(high <= UINT32_MASK);
  assert(low <= UINT32_MASK);
  return (((uint64_t)high) << 32 | low);
}