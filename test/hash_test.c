#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
  int id;
  char name[10];
  hash_node_t hash;
};

int main(int argc, char const *argv[]) {
  hash_table_t *tbl = hash_create(10);

  for (int i = 0; i < 10; ++i) {
    struct student *stu = malloc(sizeof(struct student));
    stu->id = i;
    sprintf(stu->name, "name%d", i);
    HASH_INSERT(struct student, hash, tbl, calc_fold_string(stu->name), stu);
  }

  struct student *s;
  HASH_SEARCH(hash, tbl, calc_fold_string("name6"), struct student *, s, assert(s->id == 6), s->id == 6);
  if (s != NULL) {
    printf("%d %s\n", s->id, s->name);
  }

  hash_table_free(tbl);
  return 0;
}
