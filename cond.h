/*

  cond.h

  cond utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05

  Version 1.1 2024/10/24 change to MySQL 9.1.0

  Version 1.2 2024/12/02 stable release
*/
#ifndef COND_H
#define COND_H
#include "mutex.h" // for mutex_t, ETIMEDOUT

typedef struct cond_t {
  pthread_cond_t cond;
} cond_t;

/** Initialize a condition variable */
static inline void cond_init(cond_t *cond) {
  int ret = pthread_cond_init(&cond->cond, NULL);
  wl_ad(ret == 0);
}

/** Destroys a condition variable */
static inline void cond_destroy(cond_t *cond) {
  int ret = pthread_cond_destroy(&cond->cond);
  wl_ad(ret == 0);
}

/** Wakes one thread waiting for condition variable */
static inline void cond_signal(cond_t *cond) {
  int ret = pthread_cond_signal(&cond->cond);
  wl_ad(ret == 0);
}

/** Wakes all threads waiting for condition variable */
static inline void cond_broadcast(cond_t *cond) {
  int ret = pthread_cond_broadcast(&cond->cond);
  wl_ad(ret == 0);
}

/** Wait on condition variable */
static inline void cond_wait(cond_t *cond, mutex_t *mutex) {
  int ret = pthread_cond_wait(&cond->cond, &mutex->mutex);
  wl_ad(ret == 0);
}

/** Do a timed wait on condition variable.
  @return true if timed out, false otherwise */
static inline bool cond_timedwait(cond_t *cond, mutex_t *mutex, const struct timespec *abstime) {
  int ret = pthread_cond_timedwait(&cond->cond, &mutex->mutex, abstime);
  wl_ad(ret == 0 || ret == ETIMEDOUT);
  return ret == ETIMEDOUT;
}

#endif