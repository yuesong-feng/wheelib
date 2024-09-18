/*

  sem.h

  semaphore utilities
  <semaphore.h> is not available in some environment, so we design our own
  semaphore

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05

  Version 1.1 2024/09/10 change timedwait from abstime to microseconds
*/
#ifndef SEM_H
#define SEM_H
#include "cond.h" // for cond, mutex

typedef struct semaphore_t {
  int v;
  mutex_t mutex;
  cond_t cond;
} semaphore_t;

static inline void semaphore_init(semaphore_t *semaphore, int v) {
  mutex_init(&semaphore->mutex);
  cond_init(&semaphore->cond);
  semaphore->v = v;
}

static inline void semaphore_destroy(semaphore_t *semaphore) {
  cond_destroy(&semaphore->cond);
  mutex_destroy(&semaphore->mutex);
}

static inline void semaphore_p(semaphore_t *semaphore) {
  mutex_lock(&semaphore->mutex);
  while (semaphore->v <= 0) {
    cond_wait(&semaphore->cond, &semaphore->mutex);
  }
  --(semaphore->v);
  mutex_unlock(&semaphore->mutex);
}

// return 0 if succeeded, 1 if timed out
static inline bool
semaphore_p_timedwait(semaphore_t *semaphore,
                      long time_in_usec /*timeout in microseconds*/) {
  bool ret = 0;
  mutex_lock(&semaphore->mutex);
  while (semaphore->v <= 0) {
    ret = cond_timedwait(&semaphore->cond, &semaphore->mutex, time_in_usec);
    if (ret != 0) // timed out
      break;
  }
  if (ret == 0) // succeeded
    --(semaphore->v);
  mutex_unlock(&semaphore->mutex);
  return ret;
}

static inline void semaphore_v(semaphore_t *semaphore) {
  mutex_lock(&semaphore->mutex);
  ++(semaphore->v);
  cond_signal(&semaphore->cond);
  mutex_unlock(&semaphore->mutex);
}

#endif