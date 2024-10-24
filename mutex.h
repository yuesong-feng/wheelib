/*

  mutex.h

  mutex utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05

  Version 1.1 2024/10/24 change to MySQL 9.1.0
*/
#ifndef MUTEX_H
#define MUTEX_H
#include "basic.h"   // for ast
#include <errno.h>   // for EBUSY
#include <pthread.h> // for pthread_mutex
#include <stdbool.h> // for bool

typedef struct mutex_t {
  pthread_mutex_t mutex;
} mutex_t;

/** Create the mutex by calling the system functions. */
static inline void mutex_init(mutex_t *mutex) {
  int ret = pthread_mutex_init(&mutex->mutex, NULL);
  ast(ret == 0);
}

/** Destroy the mutex */
static inline void mutex_destroy(mutex_t *mutex) {
  int ret = pthread_mutex_destroy(&mutex->mutex);
  astd(ret == 0);
}

/** Acquire the mutex. */
static inline void mutex_lock(mutex_t *mutex) {
  int ret = pthread_mutex_lock(&mutex->mutex);
  ast(ret == 0);
}

/** @return true if locking succeeded */
static inline bool mutex_trylock(mutex_t *mutex) {
  int ret = pthread_mutex_trylock(&mutex->mutex);
  astd(ret == 0 || ret == EBUSY);
  return ret == 0;
}

/** Release the mutex. */
static inline void mutex_unlock(mutex_t *mutex) {
  int ret = pthread_mutex_unlock(&mutex->mutex);
  ast(ret == 0);
}

#endif