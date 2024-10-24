/*

  hash.h

  hash utilities, from MySQL 8.4.0 InnoBase

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05
  Version 1.1 2024/09/09 add hash_table->heap, insert_8b, delete_8b,search_8b

*/
#ifndef HASH_H
#define HASH_H
#include "mem.h" // for everything

typedef void *hash_node_t;

typedef struct hash_cell_t {
  void *node;
} hash_cell_t;

typedef struct hash_table_t {
  hash_cell_t *cells;
  size_t n_cells;
} hash_table_t;

#define HASH_INSERT(TYPE, NAME, TABLE, HASH_VALUE, NODE)                           \
  do {                                                                             \
    hash_cell_t *cell3333;                                                         \
    TYPE *struct3333;                                                              \
    const uint64_t hash_value3333 = HASH_VALUE;                                    \
                                                                                   \
    (NODE)->NAME = NULL;                                                           \
                                                                                   \
    cell3333 = hash_get_nth_cell(TABLE, hash_calc_cell_id(hash_value3333, TABLE)); \
                                                                                   \
    if (cell3333->node == NULL) {                                                  \
      cell3333->node = NODE;                                                       \
    } else {                                                                       \
      struct3333 = (TYPE *)cell3333->node;                                         \
                                                                                   \
      while (struct3333->NAME != NULL) {                                           \
        struct3333 = (TYPE *)struct3333->NAME;                                     \
      }                                                                            \
                                                                                   \
      struct3333->NAME = NODE;                                                     \
    }                                                                              \
  } while (0)

#define HASH_DELETE(TYPE, NAME, TABLE, HASH_VALUE, NODE)                           \
  do {                                                                             \
    hash_cell_t *cell3333;                                                         \
    TYPE *struct3333;                                                              \
    const uint64_t hash_value3333 = HASH_VALUE;                                    \
                                                                                   \
    cell3333 = hash_get_nth_cell(TABLE, hash_calc_cell_id(hash_value3333, TABLE)); \
                                                                                   \
    if (cell3333->node == NODE) {                                                  \
      cell3333->node = NODE->NAME;                                                 \
    } else {                                                                       \
      struct3333 = (TYPE *)cell3333->node;                                         \
                                                                                   \
      while (struct3333->NAME != NODE) {                                           \
        struct3333 = (TYPE *)struct3333->NAME;                                     \
        ast(struct3333);                                                           \
      }                                                                            \
                                                                                   \
      struct3333->LINK = NODE->LINK;                                               \
    }                                                                              \
  } while (0)

static inline hash_cell_t *hash_get_nth_cell(hash_table_t *table, size_t n) {
  astd(table);
  astd(n < table->n_cells);
  return &table->cells[n];
}

static inline uint64_t hash_calc_cell_id(uint64_t hash_value, hash_table_t *table) {
  astd(table);
  return hash_value % table->n_cells;
}

static inline void *hash_get_first(hash_table_t *table, size_t cell_id) {
  return hash_get_nth_cell(table, cell_id)->node;
}

#define HASH_GET_NEXT(NAME, NODE) ((NODE)->NAME)

#define HASH_SEARCH(NAME, TABLE, HASH_VALUE, TYPE, NODE, COND)                      \
  {                                                                                 \
    const uint64_t hash_value3333 = HASH_VALUE;                                     \
                                                                                    \
    (NODE) = (TYPE)hash_get_first(TABLE, hash_calc_cell_id(hash_value3333, TABLE)); \
                                                                                    \
    while ((NODE) != NULL) {                                                        \
      if (COND) {                                                                   \
        break;                                                                      \
      } else {                                                                      \
        (NODE) = (TYPE)HASH_GET_NEXT(NAME, NODE);                                   \
      }                                                                             \
    }                                                                               \
  }

#define HASH_SEARCH_ALL(NAME, TABLE, TYPE, NODE, COND) \
  do {                                                 \
    size_t i3333;                                      \
                                                       \
    for (i3333 = (TABLE)->n_cells; i3333--;) {         \
      (NODE) = (TYPE)hash_get_first(TABLE, i3333);     \
                                                       \
      while ((NODE) != NULL) {                         \
        if (COND) {                                    \
          break;                                       \
        }                                              \
                                                       \
        (NODE) = (TYPE)HASH_GET_NEXT(NAME, NODE);      \
      }                                                \
                                                       \
      if ((NODE) != NULL) {                            \
        break;                                         \
      }                                                \
    }                                                  \
  } while (0)

static inline void hash_table_clear(hash_table_t *table) {
  astd(table);
  memset(table->cells, 0x0, table->n_cells * sizeof(hash_cell_t));
}

static inline hash_table_t *hash_create(size_t n) {
  uint64_t prime;
  hash_table_t *table;

  prime = find_prime(n);

  table = (hash_table_t *)mem_alloc(sizeof(hash_table_t));
  table->cells = (hash_cell_t *)mem_alloc(sizeof(hash_cell_t) * prime);
  table->n_cells = prime;

  hash_table_clear(table);

  return table;
}

static inline void hash_table_free(hash_table_t *table) {
  astd(table);
  astd(table->cells);
  mem_free(table->cells);
  mem_free(table);
}

#endif