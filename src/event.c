/**
 * @file event.c
 * @date 2025-01-25
 * @author yuesong-feng
 */
#include "event.h"
#include "sec.h"
#include <assert.h>
#include <errno.h>

void event_init(event_t *event) {
  mutex_init(&event->mutex);
  cond_init(&event->cond);
  event->is_set = false;
}

void event_destroy(event_t *event) {
  cond_destroy(&event->cond);
  mutex_destroy(&event->mutex);
}

void event_set(event_t *event) {
  mutex_lock(&event->mutex);
  if (event->is_set == false) {
    event->is_set = true;
    cond_broadcast(&event->cond);
  }
  mutex_unlock(&event->mutex);
}

void event_reset(event_t *event) {
  mutex_lock(&event->mutex);
  if (event->is_set == true)
    event->is_set = false;
  mutex_unlock(&event->mutex);
}

void event_wait(event_t *event) {
  mutex_lock(&event->mutex);
  while (event->is_set == false) {
    cond_wait(&event->cond, &event->mutex);
  }
  mutex_unlock(&event->mutex);
}

int event_timedwait(event_t *event, usec_t us) {
  int ret = 0;
  struct timeval tv;
  struct timespec ts;

  timeval_now(&tv);
  timeval_add_usec(&tv, us);
  timespec_from_timeval(&ts, &tv);

  mutex_lock(&event->mutex);
  do {
    if (event->is_set == true)
      break;
    ret = cond_timedwait(&event->cond, &event->mutex, &ts);
  } while (ret == 0);
  mutex_unlock(&event->mutex);

  assert(ret == 0 || ret == ETIMEDOUT);
  return ret;
}