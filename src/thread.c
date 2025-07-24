/**
 * @file thread.c
 * @date 2025-01-25
 * @author yuesong-feng
 */
#include "thread.h"
#include "sec.h"
#include <assert.h>
//#include <sys/select.h>

thread_t thread_create(void *(*func)(void *), void *arg) {
  pthread_t pthread;
  int ret = pthread_create(&pthread, NULL, func, arg);
  assert(ret == 0);
  return (thread_t)pthread;
}

void thread_join(thread_t thread) {
  int ret = pthread_join((pthread_t)thread, NULL);
  assert(ret == 0);
}

void thread_detach(thread_t thread) {
  int ret = pthread_detach((pthread_t)thread);
  assert(ret == 0);
}

int thread_equal(thread_t thread1, thread_t thread2) {
  return pthread_equal((pthread_t)thread1, (pthread_t)thread2);
}

thread_t thread_self() {
  return (thread_t)pthread_self();
}

void thread_exit(void *retval) {
  pthread_exit(retval);
}

void thread_yield() {
  int ret = sched_yield();
  assert(ret == 0);
}

//void thread_sleep(usec_t us) {
//  struct timeval tv;
//  timeval_from_usec(&tv, us);
//  int ret = select(0, NULL, NULL, NULL, &tv);
//  assert(ret == 0);
//}