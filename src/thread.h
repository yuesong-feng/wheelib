/**
 * @file thread.h
 * @date 2025-01-25
 * @author yuesong-feng
 */
#ifndef THREAD_H
#define THREAD_H
#include "sec.h"
#include <pthread.h>

typedef pthread_t thread_t;

thread_t thread_create(void *(*func)(void *), void *arg);

void thread_join(thread_t thread);

void thread_detach(thread_t thread);

int thread_equal(thread_t thread1, thread_t thread2);

thread_t thread_self();

void thread_exit(void *retval);

void thread_yield();

void thread_sleep(usec_t us);

#endif