/**
 * @file sec.c
 * @date 2025-01-25
 * @author yuesong-feng
 */
#include "sec.h"
#include <assert.h>

void timeval_now(struct timeval *tv) {
  int ret = gettimeofday(tv, NULL);
  assert(ret == 0);
}

void timeval_from_usec(struct timeval *tv, usec_t usec) {
  tv->tv_sec = usec / USEC_PER_SEC;
  tv->tv_usec = usec % USEC_PER_SEC;
}

void timeval_add_usec(struct timeval *tv, usec_t usec) {
  tv->tv_sec += usec / USEC_PER_SEC;
  tv->tv_usec += usec % USEC_PER_SEC;
  if (tv->tv_usec >= USEC_PER_SEC) {
    tv->tv_sec += 1;
    tv->tv_usec -= USEC_PER_SEC;
  }
  assert(tv->tv_usec < USEC_PER_SEC);
}

void timespec_from_timeval(struct timespec *ts, const struct timeval *tv) {
  ts->tv_sec = tv->tv_sec;
  ts->tv_nsec = tv->tv_usec * NSEC_PER_USEC;
}