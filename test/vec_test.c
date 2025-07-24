#include "vec.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int data;
} node;

int main(int argc, char const *argv[]) {
  vector_t vec;

  vector_init(&vec, sizeof(struct node));
  
  for (int i = 0; i < 100; ++i) {
    node *n = malloc(sizeof(node));
    n->data = i;
    vector_push_back(&vec, n);
  }
  printf("%d\n", ((node *)vector_at(&vec, 46))->data);

  return 0;
}
