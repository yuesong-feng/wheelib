/**
 * @file mutex.c
 * @date 2025-01-25
 * @author yuesong-feng
 */
#include "mutex.h"
#include <assert.h>
#include <errno.h>

void mutex_init(mutex_t *mutex) {
  int ret = pthread_mutex_init(&mutex->mutex, NULL);
  assert(ret == 0);
}

void mutex_destroy(mutex_t *mutex) {
  int ret = pthread_mutex_destroy(&mutex->mutex);
  assert(ret == 0);
}

void mutex_lock(mutex_t *mutex) {
  int ret = pthread_mutex_lock(&mutex->mutex);
  assert(ret == 0);
}

int mutex_trylock(mutex_t *mutex) {
  int ret = pthread_mutex_trylock(&mutex->mutex);
  assert(ret == 0 || ret == EBUSY);
  return ret;
}

#ifndef __APPLE__
int mutex_timedlock(mutex_t *mutex, const struct timespec *ts) {
  int ret = pthread_mutex_timedlock(&mutex->mutex, ts);
  assert(ret == 0 || ret == ETIMEDOUT);
  return ret;
}
#endif

void mutex_unlock(mutex_t *mutex) {
  int ret = pthread_mutex_unlock(&mutex->mutex);
  assert(ret == 0);
}
