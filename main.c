#include "event.h"
#include "hash.h"
#include "sem.h"
#include "thread.h"
#include "calc.h"
#include <stdio.h>

typedef struct value_t {
  int a;
  void *hash;
} value_t;

void *func1(void *arg) {
  event_t *evt = arg;
  thread_sleep(3000000);
  event_set(evt);
  return NULL;
}

void *func2(void *arg) {
  semaphore_t *sem = arg;
  thread_sleep(3000000);
  int i;
  for(i = 0; i < 10; ++i)
    semaphore_v(sem);
  return NULL;
}

int main() {
  char *buf = mem_alloc(100);

  memcpy(buf, "abc123", 5);
  memset(buf, 'a', 100);
  printf("%s\n", buf);

  mem_free(buf);

  hash_table_t *tab = hash_create(10);
  value_t v1 = {123};
  value_t v2 = {234};
  HASH_INSERT(value_t, hash, tab, 123, &v1);
  HASH_INSERT(value_t, hash, tab, 234, &v2);

  const char *str = "feng";
  hash_insert_8b(tab, 5555);
  hash_insert_8b(tab, (uint64_t)str);

  hash_8b_t *cell = hash_search_8b(tab, 5555);
  printf("%d\n", (int)cell->val);
  hash_8b_t *cell2 = hash_search_8b(tab, (uint64_t)str);
  printf("%s\n", (int)cell2->val);

  hash_delete_8b(tab, cell);
  hash_delete_8b(tab, cell2);

  cell = hash_search_8b(tab, 5555);
  printf("%p\n", cell);
  cell2 = hash_search_8b(tab, (uint64_t)str);
  printf("%p\n", cell2);

  value_t *val = NULL;
  HASH_SEARCH(hash, tab, 123, value_t *, val, val->a == 123);
  printf("%d\n", val->a);

  hash_table_free(tab);

  event_t event;
  semaphore_t sem;

  event_init(&event);
  semaphore_init(&sem, 10);

  thread_t thread;

  thread_create(func1, (void *)&event, NULL);
  thread_create(func2, (void *)&sem, NULL);

  event_wait(&event);

  int i;
  for (i = 0; i < 20; ++i)
    semaphore_p(&sem);

  event_destroy(&event);

  mem_heap_t *heap = mem_heap_create(0);
  lst_t lst;
  LIST_DATA_ADD_LAST(heap, lst, 123);

  return 0;
}
