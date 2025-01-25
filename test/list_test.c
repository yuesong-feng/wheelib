#include "list.h"
#include <stdint.h>
#include <stdio.h>
int main(int argc, char const *argv[]) {
  alloc_t *alloc = alloc_create(alloc_os);
  list_t *list = list_create(alloc);
  list_add_last(list, (void *)123);
  printf("%d\n", (int)(intptr_t)list_get_first(list)->data);
  list_free(list);
  alloc_free(alloc);
  return 0;
}
