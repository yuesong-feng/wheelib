/**
 * @file cond.h
 * @date 2025-01-25
 * @author yuesong-feng
 */
#ifndef COND_H
#define COND_H
#include "mutex.h"

typedef struct cond {
  pthread_cond_t cond;
} cond_t;

void cond_init(cond_t *cond);

void cond_destroy(cond_t *cond);

void cond_signal(cond_t *cond);

void cond_broadcast(cond_t *cond);

void cond_wait(cond_t *cond, mutex_t *mutex);

int cond_timedwait(cond_t *cond, mutex_t *mutex, const struct timespec *ts);

#endif