/**
 * @file sema.c
 * @date 2025-01-25
 * @author yuesong-feng
 */
#include "sema.h"
#include <assert.h>
#include <errno.h>

void sema_init(sema_t *sema, int value) {
  mutex_init(&sema->mutex);
  cond_init(&sema->cond);
  sema->value = value;
}

void sema_destroy(sema_t *sema) {
  cond_destroy(&sema->cond);
  mutex_destroy(&sema->mutex);
}

void sema_post(sema_t *sema) {
  mutex_lock(&sema->mutex);
  sema->value++;
  cond_signal(&sema->cond);
  mutex_unlock(&sema->mutex);
}

void sema_wait(sema_t *sema) {
  mutex_lock(&sema->mutex);
  while (sema->value <= 0) {
    cond_wait(&sema->cond, &sema->mutex);
  }
  sema->value--;
  mutex_unlock(&sema->mutex);
}

int sema_trywait(sema_t *sema) {
  int ret = 0;
  mutex_lock(&sema->mutex);
  if (sema->value > 0) {
    sema->value--;
  } else {
    ret = -1;
    errno = EAGAIN;
  }
  mutex_unlock(&sema->mutex);
  return ret;
}

int sema_timedwait(sema_t *sema, usec_t us) {
  int ret = 0;
  struct timeval tv;
  struct timespec ts;

  timeval_now(&tv);
  timeval_add_usec(&tv, us);
  timespec_from_timeval(&ts, &tv);

  mutex_lock(&sema->mutex);
  while (sema->value <= 0) {
    ret = cond_timedwait(&sema->cond, &sema->mutex, &ts);
    if (ret != 0) {
      errno = ETIMEDOUT;
      break;
    }
  }
  if (ret == 0) {
    sema->value--;
  }
  mutex_unlock(&sema->mutex);
  assert(ret == 0 || ret == ETIMEDOUT);
  return ret == 0 ? 0 : -1;
}