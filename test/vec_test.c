#include "vec.h"
#include <stdint.h>
#include <stdio.h>

typedef struct node {
  int data;
} node;

int main(int argc, char const *argv[]) {
  alloc_t *alloc = alloc_create(alloc_heap);
  vector_t *vec = vector_create(alloc, 4, 100);
  for (int i = 0; i < 100; ++i) {
    node *n = alloc->malloc(alloc, sizeof(node));
    n->data = i;
    vector_push(vec, n);
  }
  printf("%d\n", ((node *)vector_get(vec, 46))->data);
  vector_free(vec);
  alloc_free(alloc);

  return 0;
}
