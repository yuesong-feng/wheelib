/**
 * @file cond.c
 * @date 2025-01-25
 * @author yuesong-feng
 */
#include "cond.h"
#include <assert.h>
#include <errno.h>

void cond_init(cond_t *cond) {
  int ret = pthread_cond_init(&cond->cond, NULL);
  assert(ret == 0);
}

void cond_destroy(cond_t *cond) {
  int ret = pthread_cond_destroy(&cond->cond);
  assert(ret == 0);
}

void cond_signal(cond_t *cond) {
  int ret = pthread_cond_signal(&cond->cond);
  assert(ret == 0);
}

void cond_broadcast(cond_t *cond) {
  int ret = pthread_cond_broadcast(&cond->cond);
  assert(ret == 0);
}

void cond_wait(cond_t *cond, mutex_t *mutex) {
  int ret = pthread_cond_wait(&cond->cond, &mutex->mutex);
  assert(ret == 0);
}

int cond_timedwait(cond_t *cond, mutex_t *mutex, const struct timespec *ts) {
  int ret = pthread_cond_timedwait(&cond->cond, &mutex->mutex, ts);
  assert(ret == 0 || ret == ETIMEDOUT);
  return ret;
}