/*

  rwlock.h

  rwlock utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/09

*/
#ifndef RWLOCK_H
#define RWLOCK_H
#include "basic.h"   // for halt_if
#include <errno.h>   // for EBUSY and ETIMEDOUT
#include <pthread.h> // for pthread_rwlock
#include <stdbool.h> // for bool

typedef struct rwlock_t {
  pthread_rwlock_t rwlock;
} rwlock_t;

static inline void rwlock_init(rwlock_t *rwlock) {
  int ret = pthread_rwlock_init(&rwlock->rwlock, NULL);
  halt_if(ret != 0);
}

static inline void rwlock_destroy(rwlock_t *rwlock) {
  int ret = pthread_rwlock_destroy(&rwlock->rwlock);
  halt_if(ret != 0);
}

static inline void rwlock_rdlock(rwlock_t *rwlock) {
  int ret = pthread_rwlock_rdlock(&rwlock->rwlock);
  halt_if(ret != 0);
}

// return 0 if succeeded, 1 if EBUSY
static inline bool rwlock_tryrdlock(rwlock_t *rwlock) {
  int ret = pthread_rwlock_tryrdlock(&rwlock->rwlock);
  halt_if(ret != 0 && ret != EBUSY);
  return ret == EBUSY;
}

// return 0 if succeeded, 1 if timed out
static inline bool rwlock_timedrdlock(rwlock_t *rwlock, const struct timespec *abstime) {
  int ret = pthread_rwlock_timedrdlock(&rwlock->rwlock, abstime);
  halt_if(ret != 0 && ret != ETIMEDOUT);
  return ret == ETIMEDOUT;
}

static inline void rwlock_wrlock(rwlock_t *rwlock) {
  int ret = pthread_rwlock_wrlock(&rwlock->rwlock);
  halt_if(ret != 0);
}

// return 0 if succeeded, 1 if EBUSY
static inline bool rwlock_trywrlock(rwlock_t *rwlock) {
  int ret = pthread_rwlock_trywrlock(&rwlock->rwlock);
  halt_if(ret != 0 && ret != EBUSY);
  return ret == EBUSY;
}

// return 0 if succeeded, 1 if timed out
static inline bool rwlock_timedwrlock(rwlock_t *rwlock, const struct timespec *abstime) {
  int ret = pthread_rwlock_timedwrlock(&rwlock->rwlock, abstime);
  halt_if(ret != 0 && ret != ETIMEDOUT);
  return ret == ETIMEDOUT;
}

static inline void rwlock_unlock(rwlock_t *rwlock) {
  int ret = pthread_rwlock_unlock(&rwlock->rwlock);
  halt_if(ret != 0);
}

#endif