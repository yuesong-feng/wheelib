/*

  mutex.h

  mutex utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05

*/
#ifndef MUTEX_H
#define MUTEX_H
#include "basic.h"   // for halt_if
#include <errno.h>   // for EBUSY
#include <pthread.h> // for pthread_mutex
#include <stdbool.h> // for bool

typedef struct mutex_t {
  pthread_mutex_t mutex;
} mutex_t;

static inline void mutex_init(mutex_t *mutex) {
  int ret = pthread_mutex_init(&mutex->mutex, NULL);
  halt_if(ret != 0);
}

static inline void mutex_destroy(mutex_t *mutex) {
  int ret = pthread_mutex_destroy(&mutex->mutex);
  halt_if(ret != 0);
}

static inline void mutex_lock(mutex_t *mutex) {
  int ret = pthread_mutex_lock(&mutex->mutex);
  halt_if(ret != 0);
}

// return 0 if lock succeeded, 1 if EBUSY
static inline bool mutex_trylock(mutex_t *mutex) {
  int ret = pthread_mutex_trylock(&mutex->mutex);
  halt_if(ret != 0 && ret != EBUSY);
  return ret == EBUSY;
}

static inline void mutex_unlock(mutex_t *mutex) {
  int ret = pthread_mutex_unlock(&mutex->mutex);
  halt_if(ret != 0);
}

#endif