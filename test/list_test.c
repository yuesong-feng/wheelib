#include "list.h"
#include <stdint.h>
#include <stdio.h>
int main(int argc, char const *argv[]) {
  list_t *list = list_create();
  list_add_last(list, (void *)123);
  printf("%d\n", (int)(intptr_t)list_get_first(list)->data);
  list_free(list);
  return 0;
}
