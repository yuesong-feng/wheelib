/**
 * @file mutex.h
 * @date 2025-01-25
 * @author yuesong-feng
 */
#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

typedef struct mutex_t mutex_t;
struct mutex_t {
  pthread_mutex_t mutex;
};

void mutex_init(mutex_t *mutex);

void mutex_destroy(mutex_t *mutex);

void mutex_lock(mutex_t *mutex);

int mutex_trylock(mutex_t *mutex);

#ifndef __APPLE__
int mutex_timedlock(mutex_t *mutex, const struct timespec *ts);
#endif

void mutex_unlock(mutex_t *mutex);

#endif