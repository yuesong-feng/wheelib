/**
 * @file rwlock.c
 * @date 2025-01-25
 * @author yuesong-feng
 */
#include "rwlock.h"
#include <assert.h>
#include <errno.h>

void rwlock_init(rwlock_t *rwlock) {
  int ret = pthread_rwlock_init(&rwlock->rwlock, NULL);
  assert(ret == 0);
}

void rwlock_destroy(rwlock_t *rwlock) {
  int ret = pthread_rwlock_destroy(&rwlock->rwlock);
  assert(ret == 0);
}

void rwlock_rdlock(rwlock_t *rwlock) {
  int ret = pthread_rwlock_rdlock(&rwlock->rwlock);
  assert(ret == 0);
}

int rwlock_tryrdlock(rwlock_t *rwlock) {
  int ret = pthread_rwlock_tryrdlock(&rwlock->rwlock);
  assert(ret == 0 || ret == EBUSY);
  return ret;
}

#ifndef __APPLE__
int rwlock_timedrdlock(rwlock_t *rwlock, const struct timespec *ts) {
  int ret = pthread_rwlock_timedrdlock(&rwlock->rwlock, ts);
  assert(ret == 0 || ret == ETIMEDOUT);
  return ret;
}
#endif

void rwlock_wrlock(rwlock_t *rwlock) {
  int ret = pthread_rwlock_wrlock(&rwlock->rwlock);
  assert(ret == 0);
}

int rwlock_trywrlock(rwlock_t *rwlock) {
  int ret = pthread_rwlock_trywrlock(&rwlock->rwlock);
  assert(ret == 0 || ret == EBUSY);
  return ret;
}

#ifndef __APPLE__
int rwlock_timedwrlock(rwlock_t *rwlock, const struct timespec *ts) {
  int ret = pthread_rwlock_timedwrlock(&rwlock->rwlock, ts);
  assert(ret == 0 || ret == ETIMEDOUT);
  return ret;
}
#endif

void rwlock_unlock(rwlock_t *rwlock) {
  int ret = pthread_rwlock_unlock(&rwlock->rwlock);
  assert(ret == 0);
}