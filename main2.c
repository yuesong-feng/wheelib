#include "event.h"
#include "sem.h"
#include "stack.h"
#include "stdio.h"

int main() {
  stack_t *stk = stack_create();
  int aaa = 345;

  stack_push_8b(stk, 123);
  stack_push_8b(stk, 234);
  stack_push_8b(stk, (uint64_t)&aaa);

  int *v3 = (int *)stack_pop_8b(stk);
  int v2 = stack_pop_8b(stk);
  int v1 = stack_pop_8b(stk);

  printf("%d %d %d\n", v1, v2, *v3);

  const char *buf = "asdf";

  stack_push_8b(stk, (uint64_t)buf);

  char *buf2 = (char *)stack_pop_8b(stk);

  printf("%s\n", buf2);

  int ret;
  event_t evt;
  event_init(&evt);
  ret = event_timedwait(&evt, 3000000);
  printf("ret = %d\n", ret);
  event_destroy(&evt);

  semaphore_t sem;
  event_init(&evt);
  semaphore_init(&sem, 1);
  ret = semaphore_p_timedwait(&sem, 3000000);
  printf("ret = %d\n", ret);
  semaphore_destroy(&sem);

  return 0;
}