/*

  event.h

  event utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05

  Version 1.1 2024/09/10 change timedwait from abstime to microseconds
*/
#ifndef EVENT_H
#define EVENT_H
#include "cond.h" // for cond_t, mutex_t

typedef struct event_t {
  bool is_set;
  mutex_t mutex;
  cond_t cond;
} event_t;

static inline void event_init(event_t *event) {
  mutex_init(&event->mutex);
  cond_init(&event->cond);
  event->is_set = false;
}

static inline void event_destroy(event_t *event) {
  cond_destroy(&event->cond);
  mutex_destroy(&event->mutex);
}

static inline void event_set(event_t *event) {
  mutex_lock(&event->mutex);
  if (!event->is_set) {
    event->is_set = true;
    cond_broadcast(&event->cond);
  }
  mutex_unlock(&event->mutex);
}

// return 0 if succeeded, 1 if EBUSY
static inline bool event_tryset(event_t *event) {
  if (mutex_trylock(&event->mutex) == 0) {
    if (!event->is_set) {
      event->is_set = true;
      cond_broadcast(&event->cond);
    }
    mutex_unlock(&event->mutex);
    return 0;
  }
  return 1;
}

static inline void event_reset(event_t *event) {
  mutex_lock(&event->mutex);
  if (event->is_set)
    event->is_set = false;
  mutex_unlock(&event->mutex);
}

static inline void event_wait(event_t *event) {
  mutex_lock(&event->mutex);
  while (!event->is_set)
    cond_wait(&event->cond, &event->mutex);
  mutex_unlock(&event->mutex);
}

// return 0 if succeeded, 1 if timed out
static inline bool
event_timedwait(event_t *event, long time_in_usec /*timeout in microseconds*/) {
  bool ret = 0;
  struct timespec abstime;
  struct timeval tv;
  const long MICROSECS_IN_A_SECOND = 1000000;

  ret = gettimeofday(&tv, NULL);
  halt_if(ret != 0);

  tv.tv_usec += time_in_usec;
  if (tv.tv_usec >= MICROSECS_IN_A_SECOND) {
    tv.tv_sec += time_in_usec / MICROSECS_IN_A_SECOND;
    tv.tv_usec %= MICROSECS_IN_A_SECOND;
  }

  abstime.tv_sec = tv.tv_sec;
  abstime.tv_nsec = tv.tv_usec * 1000;
  halt_if(abstime.tv_nsec < 999999999);

  mutex_lock(&event->mutex);
  if (event->is_set == false)
    ret = cond_timedwait(&event->cond, &event->mutex, &abstime);
  mutex_unlock(&event->mutex);
  return ret;
}

#endif