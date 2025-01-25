/**
 * @file rwlock.h
 * @date 2025-01-25
 * @author yuesong-feng
 */
#ifndef RWLOCK_H
#define RWLOCK_H

#include <pthread.h>

typedef struct rwlock_t rwlock_t;
struct rwlock_t {
  pthread_rwlock_t rwlock;
};

void rwlock_init(rwlock_t *rwlock);

void rwlock_destroy(rwlock_t *rwlock);

void rwlock_rdlock(rwlock_t *rwlock);

int rwlock_tryrdlock(rwlock_t *rwlock);

#ifndef __APPLE__
int rwlock_timedrdlock(rwlock_t *rwlock, const struct timespec *ts);
#endif

void rwlock_wrlock(rwlock_t *rwlock);

int rwlock_trywrlock(rwlock_t *rwlock);

#ifndef __APPLE__
int rwlock_timedwrlock(rwlock_t *rwlock, const struct timespec *ts);
#endif

void rwlock_unlock(rwlock_t *rwlock);

#endif