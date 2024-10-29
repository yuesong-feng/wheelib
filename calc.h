/*

  calc.h

  calc utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05

  Version 1.1 2024/09/18 add calc_abstime
*/
#ifndef CALC_H
#define CALC_H
#include "basic.h"    // for unlikely
#include <stdbool.h>  // for bool
#include <stdint.h>   // for (u)int(n)_t
#include <sys/time.h> // for struct timeval
#include <time.h>     // for calc_abstime
#include "dbg.h"

/** Calculates fast the remainder of n/m when m is a power of two.
 @param n in: numerator
 @param m in: denominator, must be a power of two
 @return the remainder of n/m */
#define calc_2pow_remainder(n, m) ((n) & ((m)-1))

/** Calculates the biggest multiple of m that is not bigger than n
 when m is a power of two.  In other words, rounds n down to m * k.
 @param n in: number to round down
 @param m in: alignment, must be a power of two
 @return n rounded down to the biggest possible integer multiple of m */
#define calc_2pow_round(n, m) ((n) & ~((m)-1))

/** Align a number down to a multiple of a power of two.
@param n in: number to round down
@param m in: alignment, must be a power of two
@return n rounded down to the biggest possible integer multiple of m */
#define calc_align_down(n, m) wl_2pow_round(n, m)

/** Calculates the smallest multiple of m that is not smaller than n
 when m is a power of two.  In other words, rounds n up to m * k.
 @param n in: number to round up
 @param m in: alignment, must be a power of two
 @return n rounded up to the smallest possible integer multiple of m */
#define calc_align(n, m) (((n) + ((m)-1)) & ~((m)-1))

/** Calculates fast the 2-logarithm of a number, rounded upward to an
 integer.
 @return logarithm in the base 2, rounded upward */
static inline long calc_2_log(long n) /*!< in: number != 0 */
{
  long res = 0;

  wl_ad(n > 0);

  n = n - 1;

  for (;;) {
    n = n / 2;

    if (n == 0) {
      break;
    }

    res++;
  }

  return (res + 1);
}

/** Calculates 2 to power n.
@param[in]      n       power of 2
@return 2 to power n */
static inline uint32_t calc_2_exp(uint32_t n) { return (1 << n); }

/** Calculates fast the number rounded up to the nearest power of 2.
 @return first power of 2 which is >= n */
long calc_2_power_up(long n) /*!< in: number != 0 */
{
  long res;

  res = 1;

  wl_ad(n > 0);

  while (res < n) {
    res = res * 2;
  }

  return (res);
}

/** Determine how many bytes (groups of 8 bits) are needed to
store the given number of bits.
@param b in: bits
@return number of bytes (octets) needed to represent b */
#define BITS_IN_BYTES(b) (((b) + 7UL) / 8UL)

/** Determines if a number is zero or a power of two.
@param[in]      n       number
@return nonzero if n is zero or a power of two; zero otherwise */
#define calc_is_2pow(n) likely(!((n) & ((n)-1)))










/////////






#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

#define overflow_if(cond) \
  if (unlikely(cond))     \
    return true;

// from PosrgreSQL
static inline bool int8_add_overflow(int8_t a, int8_t b, int8_t *result) {
  int16_t res = (int16_t)a + (int16_t)b;
  overflow_if(res > INT8_MAX || res < INT8_MIN);
  *result = (int8_t)res;
  return false;
}

static inline bool int16_add_overflow(int16_t a, int16_t b, int16_t *result) {
  int32_t res = (int32_t)a + (int32_t)b;
  overflow_if(res > INT16_MAX || res < INT16_MIN);
  *result = (int16_t)res;
  return false;
}

static inline bool int32_add_overflow(int32_t a, int32_t b, int32_t *result) {
  int64_t res = (int64_t)a + (int64_t)b;
  overflow_if(res > INT32_MAX || res < INT32_MIN);
  *result = (int32_t)res;
  return false;
}

static inline bool int64_add_overflow(int64_t a, int64_t b, int64_t *result) {
  overflow_if((a > 0 && b > 0 && a > INT64_MAX - b) ||
              (a < 0 && b < 0 && a < INT64_MIN - b));
  *result = a + b;
  return false;
}

static inline bool uint8_add_overflow(uint8_t a, uint8_t b, uint8_t *result) {
  uint8_t res = a + b;
  overflow_if(res < a);
  *result = res;
  return false;
}

static inline bool uint16_add_overflow(uint16_t a, uint16_t b, uint16_t *result) {
  uint16_t res = a + b;
  overflow_if(res < a);
  *result = res;
  return false;
}

static inline bool uint32_add_overflow(uint32_t a, uint32_t b, uint32_t *result) {
  uint32_t res = a + b;
  overflow_if(res < a);
  *result = res;
  return false;
}

static inline bool uint64_add_overflow(uint64_t a, uint64_t b, uint64_t *result) {
  uint64_t res = a + b;
  overflow_if(res < a);
  *result = res;
  return false;
}

static inline bool int8_sub_overflow(int8_t a, int8_t b, int8_t *result) {
  int16_t res = (int16_t)a - (int16_t)b;
  overflow_if(res > INT8_MAX || res < INT8_MIN);
  *result = (int8_t)res;
  return false;
}

static inline bool int16_sub_overflow(int16_t a, int16_t b, int16_t *result) {
  int32_t res = (int32_t)a - (int32_t)b;
  overflow_if(res > INT16_MAX || res < INT16_MIN);
  *result = (int16_t)res;
  return false;
}

static inline bool int32_sub_overflow(int32_t a, int32_t b, int32_t *result) {
  int64_t res = (int64_t)a - (int64_t)b;
  overflow_if(res > INT32_MAX || res < INT32_MIN);
  *result = (int32_t)res;
  return false;
}

static inline bool int64_sub_overflow(int64_t a, int64_t b, int64_t *result) {
  overflow_if((a < 0 && b > 0 && a < INT64_MIN + b) ||
              (a >= 0 && b < 0 && a > INT64_MAX + b));
  *result = a - b;
  return false;
}

static inline bool uint8_sub_overflow(uint8_t a, uint8_t b, uint8_t *result) {
  overflow_if(b > a);
  *result = a - b;
  return false;
}

static inline bool uint16_sub_overflow(uint16_t a, uint16_t b, uint16_t *result) {
  overflow_if(b > a);
  *result = a - b;
  return false;
}

static inline bool uint32_sub_overflow(uint32_t a, uint32_t b, uint32_t *result) {
  overflow_if(b > a);
  *result = a - b;
  return false;
}

static inline bool uint64_sub_overflow(uint64_t a, uint64_t b, uint64_t *result) {
  overflow_if(b > a);
  *result = a - b;
  return false;
}

static inline bool int8_mul_overflow(int8_t a, int8_t b, int8_t *result) {
  int16_t res = (int16_t)a * (int16_t)b;
  overflow_if(res > INT8_MAX || res < INT8_MIN);
  *result = (int8_t)res;
  return false;
}

static inline bool int16_mul_overflow(int16_t a, int16_t b, int16_t *result) {
  int32_t res = (int32_t)a * (int32_t)b;
  overflow_if(res > INT16_MAX || res < INT16_MIN);
  *result = (int16_t)res;
  return false;
}

static inline bool int32_mul_overflow(int32_t a, int32_t b, int32_t *result) {
  int64_t res = (int64_t)a * (int64_t)b;
  overflow_if(res > INT32_MAX || res < INT32_MIN);
  *result = (int32_t)res;
  return false;
}

static inline bool int64_mul_overflow(int64_t a, int64_t b, int64_t *result) {
  overflow_if((a > INT32_MAX || a < INT32_MIN || b > INT32_MAX || b < INT32_MIN) &&
              a != 0 && a != 1 && b != 0 && b != 1 &&
              ((a > 0 && b > 0 && a > INT64_MAX / b) ||
               (a > 0 && b < 0 && b < INT64_MIN / a) ||
               (a < 0 && b > 0 && a < INT64_MIN / b) ||
               (a < 0 && b < 0 && a < INT64_MAX / b)));
  *result = a * b;
  return false;
}

static inline bool uint8_mul_overflow(uint8_t a, uint8_t b, uint8_t *result) {
  uint16_t res = (uint16_t)a * (uint16_t)b;
  overflow_if(res > UINT8_MAX);
  *result = (uint8_t)res;
  return false;
}

static inline bool uint16_mul_overflow(uint16_t a, uint16_t b, uint16_t *result) {
  uint32_t res = (uint32_t)a * (uint32_t)b;
  overflow_if(res > UINT16_MAX);
  *result = (uint16_t)res;
  return false;
}

static inline bool uint32_mul_overflow(uint32_t a, uint32_t b, uint32_t *result) {
  uint64_t res = (uint64_t)a * (uint64_t)b;
  overflow_if(res > UINT32_MAX);
  *result = (uint32_t)res;
  return false;
}

static inline bool uint64_mul_overflow(uint64_t a, uint64_t b, uint64_t *result) {
  uint64_t res = a * b;
  overflow_if(a != 0 && b != res / a);
  *result = res;
  return false;
}

// from MySQL 9.1.0 InnoBase
static inline uint64_t find_prime(uint64_t n) {
  uint64_t pow2;
  uint64_t i;

  const double random1 = 1.0412321;
  const double random2 = 1.1131347;
  const double random3 = 1.0132677;

  n += 100;

  pow2 = 1;
  while (pow2 * 2 < n) {
    pow2 = 2 * pow2;
  }

  if ((double)n < 1.05 * (double)pow2) {
    n = (uint64_t)((double)n * random1);
  }

  pow2 = 2 * pow2;

  if ((double)n > 0.95 * (double)pow2) {
    n = (uint64_t)((double)n * random2);
  }

  if (n > pow2 - 20) {
    n += 30;
  }

  /* Now we have n far enough from powers of 2. To make
  n more random (especially, if it was not near
  a power of 2), we then multiply it by a random number. */

  n = (uint64_t)((double)n * random3);

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

  return n;
}

// used by event_timedwait and semaphore_p_timedwait
// convert microseconds to abstime
#define INFINITE_TIME ((unsigned long)(-1)) // ULINT_UNDEFINED
static inline void calc_abstime(unsigned long time_in_usec /*timeout in microseconds, or INFINITE_TIME*/, struct timespec *abstime) {
  const unsigned long MICROSECS_IN_A_SECOND = 1000000;
  if (time_in_usec != INFINITE_TIME) {
    struct timeval tv;
    int ret;

    ret = gettimeofday(&tv, NULL);
    wl_a(ret == 0);

    tv.tv_usec += time_in_usec;

    if ((unsigned long)tv.tv_usec >= MICROSECS_IN_A_SECOND) {
      tv.tv_sec += time_in_usec / MICROSECS_IN_A_SECOND;
      tv.tv_usec %= MICROSECS_IN_A_SECOND;
    }

    abstime->tv_sec = tv.tv_sec;
    abstime->tv_nsec = tv.tv_usec * 1000;
  } else {
    abstime->tv_nsec = 999999999;
    abstime->tv_sec = (time_t)(unsigned long)(-2); // ULINT_MAX
  }
  wl_a(abstime->tv_nsec <= 999999999);
}

#endif