/**
 * @file hash.h
 * @date 2025-01-29
 * @author yuesong-feng
 */
#ifndef HASH_H
#define HASH_H
#include "calc.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

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

static inline void hash_table_clear(hash_table_t *table) {
  assert(table);
  memset(table->array, 0x0, table->n_cells * sizeof(*table->array));
}

static inline hash_table_t *hash_create(size_t n) {
  hash_cell_t *array;
  size_t prime;
  hash_table_t *table;

  prime = calc_find_prime(n);

  table = (hash_table_t *)malloc(sizeof(hash_cell_t));
  assert(table);

  array = (hash_cell_t *)malloc(sizeof(hash_cell_t) * prime);
  assert(array);

  table->array = array;
  table->n_cells = prime;

  hash_table_clear(table);

  return table;
}

static inline void hash_table_free(hash_table_t *table) {
  assert(table);
  free(table->array);
  free(table);
}

#define HASH_RANDOM_MASK2 1653893711
static inline size_t hash_calc_hash(size_t fold, hash_table_t *table) {
  assert(table);
  fold = fold ^ HASH_RANDOM_MASK2;
  return fold % table->n_cells;
}

static inline hash_cell_t *hash_get_nth_cell(hash_table_t *table, size_t n) {
  assert(table);
  assert(n < table->n_cells);
  return (table->array + n);
}

#define HASH_ASSERT_VALID(DATA) assert((void *)(DATA) != (void *)-1)
#define HASH_INVALIDATE(DATA, NAME) *(void **)(&DATA->NAME) = (void *)-1

#define HASH_INSERT(TYPE, NAME, TABLE, FOLD, DATA)                    \
  do {                                                                \
    hash_cell_t *cell3333;                                            \
    TYPE *struct3333;                                                 \
                                                                      \
    (DATA)->NAME = NULL;                                              \
                                                                      \
    cell3333 = hash_get_nth_cell(TABLE, hash_calc_hash(FOLD, TABLE)); \
                                                                      \
    if (cell3333->node == NULL) {                                     \
      cell3333->node = DATA;                                          \
    } else {                                                          \
      struct3333 = (TYPE *)cell3333->node;                            \
                                                                      \
      while (struct3333->NAME != NULL) {                              \
                                                                      \
        struct3333 = (TYPE *)struct3333->NAME;                        \
      }                                                               \
                                                                      \
      struct3333->NAME = DATA;                                        \
    }                                                                 \
  } while (0)

#define HASH_DELETE(TYPE, NAME, TABLE, FOLD, DATA)                    \
  do {                                                                \
    hash_cell_t *cell3333;                                            \
    TYPE *struct3333;                                                 \
                                                                      \
    cell3333 = hash_get_nth_cell(TABLE, hash_calc_hash(FOLD, TABLE)); \
                                                                      \
    if (cell3333->node == DATA) {                                     \
      cell3333->node = DATA->NAME;                                    \
    } else {                                                          \
      struct3333 = (TYPE *)cell3333->node;                            \
                                                                      \
      while (struct3333->NAME != DATA) {                              \
                                                                      \
        struct3333 = (TYPE *)struct3333->NAME;                        \
        assert(struct3333);                                           \
      }                                                               \
                                                                      \
      struct3333->NAME = DATA->NAME;                                  \
    }                                                                 \
    HASH_INVALIDATE(DATA, NAME);                                      \
  } while (0)

#define HASH_GET_FIRST(TABLE, HASH_VAL) (hash_get_nth_cell(TABLE, HASH_VAL)->node)

#define HASH_GET_NEXT(NAME, DATA) ((DATA)->NAME)

#define HASH_SEARCH(NAME, TABLE, FOLD, TYPE, DATA, ASSERTION, TEST)    \
  {                                                                    \
    (DATA) = (TYPE)HASH_GET_FIRST(TABLE, hash_calc_hash(FOLD, TABLE)); \
    HASH_ASSERT_VALID(DATA);                                           \
                                                                       \
    while ((DATA) != NULL) {                                           \
      ASSERTION;                                                       \
      if (TEST) {                                                      \
        break;                                                         \
      } else {                                                         \
        HASH_ASSERT_VALID(HASH_GET_NEXT(NAME, DATA));                  \
        (DATA) = (TYPE)HASH_GET_NEXT(NAME, DATA);                      \
      }                                                                \
    }                                                                  \
  }

#define HASH_SEARCH_ALL(NAME, TABLE, TYPE, DATA, ASSERTION, TEST) \
  do {                                                            \
    size_t i3333;                                                 \
                                                                  \
    for (i3333 = (TABLE)->n_cells; i3333--;) {                    \
      (DATA) = (TYPE)HASH_GET_FIRST(TABLE, i3333);                \
                                                                  \
      while ((DATA) != NULL) {                                    \
        HASH_ASSERT_VALID(DATA);                                  \
        ASSERTION;                                                \
                                                                  \
        if (TEST) {                                               \
          break;                                                  \
        }                                                         \
                                                                  \
        (DATA) = (TYPE)HASH_GET_NEXT(NAME, DATA);                 \
      }                                                           \
                                                                  \
      if ((DATA) != NULL) {                                       \
        break;                                                    \
      }                                                           \
    }                                                             \
  } while (0)

#define HASH_MIGRATE(OLD_TABLE, NEW_TABLE, NODE_TYPE, PTR_NAME, FOLD_FUNC) \
  do {                                                                     \
    size_t i2222;                                                          \
    size_t cell_count2222;                                                 \
                                                                           \
    cell_count2222 = hash_get_n_cells(OLD_TABLE);                          \
                                                                           \
    for (i2222 = 0; i2222 < cell_count2222; i2222++) {                     \
      NODE_TYPE *node2222 = HASH_GET_FIRST((OLD_TABLE), i2222);            \
                                                                           \
      while (node2222) {                                                   \
        NODE_TYPE *next2222 = node2222->PTR_NAME;                          \
        size_t fold2222 = FOLD_FUNC(node2222);                             \
                                                                           \
        HASH_INSERT(NODE_TYPE, PTR_NAME, (NEW_TABLE),                      \
                    fold2222, node2222);                                   \
                                                                           \
        node2222 = next2222;                                               \
      }                                                                    \
    }                                                                      \
  } while (0)

#endif