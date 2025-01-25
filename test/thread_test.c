#include "thread.h"
#include <stdio.h>

void *func(void *arg) {
  printf("Hello, world!\n");
  thread_sleep(3 * USEC_PER_SEC);
  return NULL;
}

int main(int argc, char const *argv[]) {
  thread_t thread;

  thread = thread_create(func, NULL);

  thread_join(thread);

  printf("Hello, world!\n");

  return 0;
}
