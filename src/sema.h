/**
 * @file sema.h
 * @date 2025-01-25
 * @author yuesong-feng
 */
#ifndef SEMA_H
#define SEMA_H
#include "cond.h"
#include "sec.h"

typedef struct sema_t sema_t;
struct sema_t {
  mutex_t mutex;
  cond_t cond;
  int value;
};

void sema_init(sema_t *sema, int value);

void sema_destroy(sema_t *sema);

void sema_post(sema_t *sema);

void sema_wait(sema_t *sema);

int sema_trywait(sema_t *sema);

int sema_timedwait(sema_t *sema, usec_t us);

#endif