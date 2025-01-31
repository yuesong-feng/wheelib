#include "vec.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int data;
} node;

int main(int argc, char const *argv[]) {
  vector_t *vec = vector_create(sizeof(struct node), 100);
  for (int i = 0; i < 100; ++i) {
    node *n = malloc(sizeof(node));
    n->data = i;
    vector_push(vec, n);
  }
  printf("%d\n", ((node *)vector_get(vec, 46))->data);
  vector_free(vec);

  return 0;
}
