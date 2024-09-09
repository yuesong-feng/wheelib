/*

  cond.h

  cond utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05

*/
#ifndef COND_H
#define COND_H
#include "mutex.h" // for mutex_t, halt_if, ETIMEDOUT

typedef struct cond_t {
  pthread_cond_t cond;
} cond_t;

static inline void cond_init(cond_t *cond) {
  int ret = pthread_cond_init(&cond->cond, NULL);
  halt_if(ret != 0);
}

static inline void cond_destroy(cond_t *cond) {
  int ret = pthread_cond_destroy(&cond->cond);
  halt_if(ret != 0);
}

static inline void cond_signal(cond_t *cond) {
  int ret = pthread_cond_signal(&cond->cond);
  halt_if(ret != 0);
}

static inline void cond_broadcast(cond_t *cond) {
  int ret = pthread_cond_broadcast(&cond->cond);
  halt_if(ret != 0);
}

static inline void cond_wait(cond_t *cond, mutex_t *mutex) {
  int ret = pthread_cond_wait(&cond->cond, &mutex->mutex);
  halt_if(ret != 0);
}

// return 0 if succeeded, 1 if timed out
static inline bool cond_timedwait(cond_t *cond, mutex_t *mutex,
                                  const struct timespec *abstime) {
  int ret = pthread_cond_timedwait(&cond->cond, &mutex->mutex, abstime);
  halt_if(ret != 0 && ret != ETIMEDOUT);
  return ret == ETIMEDOUT;
}

#endif