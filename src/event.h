/**
 * @file event.h
 * @date 2025-01-25
 * @author yuesong-feng
 */
#ifndef EVENT_H
#define EVENT_H
#include "cond.h"
#include "mutex.h"
#include "sec.h"
#include <stdbool.h>

typedef struct event_t event_t;
struct event_t {
  mutex_t mutex;
  cond_t cond;
  bool is_set;
};

void event_init(event_t *event);

void event_destroy(event_t *event);

void event_set(event_t *event);

void event_reset(event_t *event);

void event_wait(event_t *event);

int event_timedwait(event_t *event, usec_t us);

#endif