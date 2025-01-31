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

bool calc_add_s8_overflow(int8_t a, int8_t b, int8_t *result) {
  int16_t res = (int16_t)a + (int16_t)b;

  if (res > CHAR_MAX || res < CHAR_MIN)
    return true;

  *result = (int8_t)res;
  return false;
}

bool calc_sub_s8_overflow(int8_t a, int8_t b, int8_t *result) {
  int16_t res = (int16_t)a - (int16_t)b;

  if (res > CHAR_MAX || res < CHAR_MIN)
    return true;

  *result = (int8_t)res;
  return false;
}

bool calc_mul_s8_overflow(int8_t a, int8_t b, int8_t *result) {
  int16_t res = (int16_t)a * (int16_t)b;

  if (res > CHAR_MAX || res < CHAR_MIN)
    return true;

  *result = (int8_t)res;
  return false;
}

bool calc_add_s16_overflow(int16_t a, int16_t b, int16_t *result) {
  int32_t res = (int32_t)a + (int32_t)b;

  if (res > SHRT_MAX || res < SHRT_MIN)
    return true;

  *result = (int16_t)res;
  return false;
}

bool calc_sub_s16_overflow(int16_t a, int16_t b, int16_t *result) {
  int32_t res = (int32_t)a - (int32_t)b;

  if (res > SHRT_MAX || res < SHRT_MIN)
    return true;

  *result = (int16_t)res;
  return false;
}

bool calc_mul_s16_overflow(int16_t a, int16_t b, int16_t *result) {
  int32_t res = (int32_t)a * (int32_t)b;

  if (res > SHRT_MAX || res < SHRT_MIN)
    return true;

  *result = (int16_t)res;
  return false;
}

bool calc_add_s32_overflow(int32_t a, int32_t b, int32_t *result) {
  int64_t res = (int64_t)a + (int64_t)b;

  if (res > INT_MAX || res < INT_MIN)
    return true;

  *result = (int32_t)res;
  return false;
}

bool calc_sub_s32_overflow(int32_t a, int32_t b, int32_t *result) {
  int64_t res = (int64_t)a - (int64_t)b;

  if (res > INT_MAX || res < INT_MIN)
    return true;

  *result = (int32_t)res;
  return false;
}

bool calc_mul_s32_overflow(int32_t a, int32_t b, int32_t *result) {
  int64_t res = (int64_t)a * (int64_t)b;

  if (res > INT_MAX || res < INT_MIN)
    return true;

  *result = (int32_t)res;
  return false;
}

bool calc_add_s64_overflow(int64_t a, int64_t b, int64_t *result) {
  if ((a > 0 && b > 0 && a > LLONG_MAX - b) ||
      (a < 0 && b < 0 && a < LLONG_MIN - b))
    return true;

  *result = a + b;
  return false;
}

bool calc_sub_s64_overflow(int64_t a, int64_t b, int64_t *result) {
  /*
   * Note: overflow is also possible when a == 0 and b < 0 (specifically,
   * when b == LLONG_MIN).
   */
  if ((a < 0 && b > 0 && a < LLONG_MIN + b) ||
      (a >= 0 && b < 0 && a > LLONG_MAX + b))
    return true;

  *result = a - b;
  return false;
}

bool calc_mul_s64_overflow(int64_t a, int64_t b, int64_t *result) {
  /*
   * Overflow can only happen if at least one value is outside the range
   * sqrt(min)..sqrt(max) so check that first as the division can be quite a
   * bit more expensive than the multiplication.
   *
   * Multiplying by 0 or 1 can't overflow of course and checking for 0
   * separately avoids any risk of dividing by 0.  Be careful about dividing
   * INT_MIN by -1 also, note reversing the a and b to ensure we're always
   * dividing it by a positive value.
   *
   */
  if ((a > INT_MAX || a < INT_MIN ||
       b > INT_MAX || b < INT_MIN) &&
      a != 0 && a != 1 && b != 0 && b != 1 &&
      ((a > 0 && b > 0 && a > LLONG_MAX / b) ||
       (a > 0 && b < 0 && b < LLONG_MIN / a) ||
       (a < 0 && b > 0 && a < LLONG_MIN / b) ||
       (a < 0 && b < 0 && a < LLONG_MAX / b)))
    return true;

  *result = a * b;
  return false;
}

bool calc_add_u8_overflow(uint8_t a, uint8_t b, uint8_t *result) {
  uint8_t res = a + b;

  if (res < a)
    return true;

  *result = res;
  return false;
}

bool calc_sub_u8_overflow(uint8_t a, uint8_t b, uint8_t *result) {
  if (b > a)
    return true;

  *result = a - b;
  return false;
}

bool calc_mul_u8_overflow(uint8_t a, uint8_t b, uint8_t *result) {
  uint16_t res = (uint16_t)a * (uint16_t)b;
  if (res > UCHAR_MAX)
    return true;

  *result = (uint8_t)res;
  return false;
}

bool calc_add_u16_overflow(uint16_t a, uint16_t b, uint16_t *result) {
  uint16_t res = a + b;

  if (res < a)
    return true;

  *result = res;
  return false;
}

bool calc_sub_u16_overflow(uint16_t a, uint16_t b, uint16_t *result) {
  if (b > a)
    return true;

  *result = a - b;
  return false;
}

bool calc_mul_u16_overflow(uint16_t a, uint16_t b, uint16_t *result) {
  uint32_t res = (uint32_t)a * (uint32_t)b;

  if (res > USHRT_MAX)
    return true;

  *result = (uint16_t)res;
  return false;
}

bool calc_add_u32_overflow(uint32_t a, uint32_t b, uint32_t *result) {
  uint32_t res = a + b;

  if (res < a)
    return true;

  *result = res;
  return false;
}

bool calc_sub_u32_overflow(uint32_t a, uint32_t b, uint32_t *result) {
  if (b > a)
    return true;

  *result = a - b;
  return false;
}

bool calc_mul_u32_overflow(uint32_t a, uint32_t b, uint32_t *result) {
  uint64_t res = (uint64_t)a * (uint64_t)b;

  if (res > UINT_MAX)
    return true;

  *result = (uint32_t)res;
  return false;
}

bool calc_add_u64_overflow(uint64_t a, uint64_t b, uint64_t *result) {
  uint64_t res = a + b;

  if (res < a)
    return true;

  *result = res;
  return false;
}

bool calc_sub_u64_overflow(uint64_t a, uint64_t b, uint64_t *result) {
  if (b > a)
    return true;

  *result = a - b;
  return false;
}

bool calc_mul_u64_overflow(uint64_t a, uint64_t b, uint64_t *result) {
  uint64_t res = a * b;

  if (a != 0 && b != res / a)
    return true;

  *result = res;
  return false;
}