#include "list.h"
#include <stdint.h>
#include <stdio.h>

bool cmp(const void *a, const void *b) {
  int x = (intptr_t)a;
  int y = (intptr_t)b;
  if (a < b)
    return true;
  return false;
}

int main(int argc, char const *argv[]) {
  list_t list;
  list_init(&list);
  list_t list2;
  list_init(&list2);

  list_push_back(&list, (void *)123);

  for (int i = 0; i < 50; ++i)
    list_push_back(&list, (void *)(intptr_t)i);
  for (int i = 25; i < 75; ++i)
    list_push_back(&list, (void *)(intptr_t)i);

  list_reverse(&list);

  list_sort1(&list, cmp);
  list_unique(&list);

  //list_merge1(&list, &list2, cmp);

  for (list_node_t *it = list_begin(&list); it != list_end(&list); it = it->next)
    printf("val: %d\n", (int)(intptr_t)it->data);

  list_destroy(&list);
  return 0;
}
