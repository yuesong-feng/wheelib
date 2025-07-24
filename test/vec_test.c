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
  
  for (int i = 0; i < 10; ++i) {
    node *n = malloc(sizeof(node));
    n->data = i;
    vector_push_back(&vec, n);
  }

  node n1 = {123};
  vector_insert(&vec, vector_at(&vec, 2), &n1);

  node n2 = {234};
  vector_insert1(&vec, vector_at(&vec, 7), 2, &n2);

  vector_erase(&vec, vector_at(&vec, 9));

  for (int i = 0; i < vector_size(&vec); ++i) {
    node *n = vector_at(&vec, i);
    printf("%d\n", n->data);
  }

  return 0;
}
