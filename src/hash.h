#ifndef HASH_H
#define HASH_H
#include "heap.h"

typedef void *hash_node_t;

typedef struct hash_cell_t hash_cell_t;
struct hash_cell_t {
  void *node;
};

typedef struct hash_table_t hash_table_t;
struct hash_table_t {
  size_t n_cells;
  hash_cell_t *array;
};

hash_table_t *hash_create(size_t size);

void hash_table_free(hash_table_t *table);

#define HASH_INSERT(TYPE, NAME, TABLE, FOLD, DATA)                    \
  do {                                                                \
    hash_cell_t *cell3333;                                            \
    TYPE *struct3333;                                                 \
    (DATA)->NAME = NULL;                                              \
    cell3333 = hash_get_nth_cell(TABLE, hash_calc_hash(FOLD, TABLE)); \
    if (cell3333->node == NULL) {                                     \
      cell3333->node = DATA;                                          \
    } else {                                                          \
      struct3333 = (TYPE *)cell3333->node;                            \
      while (struct3333->NAME != NULL) {                              \
        struct3333 = (TYPE *)struct3333->NAME;                        \
      }                                                               \
      struct3333->NAME = DATA;                                        \
    }                                                                 \
  } while (0)

#define HASH_DELETE(TYPE, NAME, TABLE, FOLD, DATA)                    \
  do {                                                                \
    hash_cell_t *cell3333;                                            \
    TYPE *struct3333;                                                 \
    cell3333 = hash_get_nth_cell(TABLE, hash_calc_hash(FOLD, TABLE)); \
    if (cell3333->node == DATA) {                                     \
      cell3333->node = DATA->NAME;                                    \
    } else {                                                          \
      struct3333 = (TYPE *)cell3333->node;                            \
      while (struct3333->NAME != DATA) {                              \
        struct3333 = (TYPE *)struct3333->NAME;                        \
      }                                                               \
      struct3333->NAME = DATA->NAME;                                  \
    }                                                                 \
  } while (0)

#define HASH_SEARCH(NAME, TABLE, FOLD, TYPE, DATA, TEST)               \
  {                                                                    \
    (DATA) = (TYPE)HASH_GET_FIRST(TABLE, hash_calc_hash(FOLD, TABLE)); \
    while ((DATA) != NULL) {                                           \
      if (TEST) {                                                      \
        break;                                                         \
      } else {                                                         \
        (DATA) = (TYPE)HASH_GET_NEXT(NAME, DATA);                      \
      }                                                                \
    }                                                                  \
  }

#endif