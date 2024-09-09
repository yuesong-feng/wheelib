/*

  thread.h

  thread utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05

*/
#ifndef THREAD_H
#define THREAD_H
#include "basic.h"      // for halt_if
#include <pthread.h>    // for pthread
#include <sys/select.h> // for select

typedef pthread_t thread_t;
typedef thread_t thread_id_t;
typedef void *(*thread_func_t)(void *);

static inline thread_t thread_create(thread_func_t func, void *arg,
                                     thread_id_t *thread_id) {
  int ret;
  thread_t pthread;

  ret = pthread_create(&pthread, NULL, func, arg);
  halt_if(ret != 0);

  if (thread_id)
    *thread_id = pthread;

  return pthread;
}

static inline void thread_join(thread_t thread) {
  int ret = pthread_join(thread, NULL);
  halt_if(ret != 0);
}

static inline void thread_exit(void *exit_value, bool detach /*If true, the thread will be detached right before exiting. If false, another thread is responsible for joining this thread.*/) {
  int ret;

  if (detach) {
    ret = pthread_detach(pthread_self());
    halt_if(ret != 0);
  }

  pthread_exit(exit_value);
}

static inline void thread_sleep(unsigned int tm /*time in microseconds*/) {
  struct timeval t;
  int ret;

  t.tv_sec = tm / 1000000;
  t.tv_usec = tm % 1000000;

  ret = select(0, NULL, NULL, NULL, &t);
  halt_if(ret == -1);
}

static inline void thread_yield() { thread_sleep(0); }

static inline thread_id_t thread_get_curr_id() { return (thread_id_t)pthread_self(); }

#endif