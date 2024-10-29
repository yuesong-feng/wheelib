/*

  rwlock.h

  rwlock utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/09

*/
#ifndef RWLOCK_H
#define RWLOCK_H
#include "basic.h"   // for ast
#include <errno.h>   // for EBUSY and ETIMEDOUT
#include <pthread.h> // for pthread_rwlock
#include <stdbool.h> // for bool

typedef struct rwlock_t {
  pthread_rwlock_t rwlock;
} rwlock_t;

static inline void rwlock_init(rwlock_t *rwlock) {
  int ret = pthread_rwlock_init(&rwlock->rwlock, NULL);
  wl_a(ret == 0);
}

static inline void rwlock_destroy(rwlock_t *rwlock) {
  int ret = pthread_rwlock_destroy(&rwlock->rwlock);
  wl_ad(ret == 0);
}

static inline void rwlock_rdlock(rwlock_t *rwlock) {
  int ret = pthread_rwlock_rdlock(&rwlock->rwlock);
  wl_a(ret == 0);
}

/** @return true if locking succeeded */
static inline bool rwlock_tryrdlock(rwlock_t *rwlock) {
  int ret = pthread_rwlock_tryrdlock(&rwlock->rwlock);
  wl_ad(ret == 0 || ret == EBUSY);
  return ret == 0;
}

/** @return true if timed out */
static inline bool rwlock_timedrdlock(rwlock_t *rwlock, const struct timespec *abstime) {
  int ret = pthread_rwlock_timedrdlock(&rwlock->rwlock, abstime);
  wl_ad(ret == 0 || ret == EBUSY);
  return ret == ETIMEDOUT;
}

static inline void rwlock_wrlock(rwlock_t *rwlock) {
  int ret = pthread_rwlock_wrlock(&rwlock->rwlock);
  wl_ad(ret == 0);
}

/** @return true if locking succeeded */
static inline bool rwlock_trywrlock(rwlock_t *rwlock) {
  int ret = pthread_rwlock_trywrlock(&rwlock->rwlock);
  wl_ad(ret == 0 || ret == EBUSY);
  return ret == 0;
}

/** @return true if timed out */
static inline bool rwlock_timedwrlock(rwlock_t *rwlock, const struct timespec *abstime) {
  int ret = pthread_rwlock_timedwrlock(&rwlock->rwlock, abstime);
  wl_ad(ret == 0 || ret == ETIMEDOUT);
  return ret == ETIMEDOUT;
}

static inline void rwlock_unlock(rwlock_t *rwlock) {
  int ret = pthread_rwlock_unlock(&rwlock->rwlock);
  wl_a(ret == 0);
}

#endif