/*

  event.h

  event utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05

  Version 1.1 2024/09/10 change timedwait from abstime to microseconds

  Version 1.2 2024/09/18 add calc_abstime

  Version 1.3 2024/10/24 change to MySQL 9.1.0

  Version 1.4 2024/12/02 stable release
*/
#ifndef EVENT_H
#define EVENT_H
#include "calc.h" // for calc_abstime
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

/** Set the event */
static inline void event_set(event_t *event) {
  mutex_lock(&event->mutex);
  if (!event->is_set) {
    event->is_set = true;
    cond_broadcast(&event->cond);
  }
  mutex_unlock(&event->mutex);
}

/** @return true if succeeded */
static inline bool event_tryset(event_t *event) {
  if (mutex_trylock(&event->mutex)) {
    if (!event->is_set) {
      event->is_set = true;
      cond_broadcast(&event->cond);
    }
    mutex_unlock(&event->mutex);
    return true;
  }
  return false;
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

/** @return true if timed out */
static inline bool
event_timedwait(event_t *event, unsigned long time_in_usec /*timeout in microseconds*/) {
  bool timed_out = false;
  struct timespec abstime;

  calc_abstime(time_in_usec, &abstime);

  mutex_lock(&event->mutex);
  do {
    if (event->is_set)
      break;
    timed_out = cond_timedwait(&event->cond, &event->mutex, &abstime);
  } while (!timed_out);
  mutex_unlock(&event->mutex);
  return timed_out ? 1 : 0;
}

#endif