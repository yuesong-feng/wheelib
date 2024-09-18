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
  mem_heap_t *heap; // only for hash_8b_t, not for cells
} hash_table_t;

#define HASH_INSERT(TYPE, LINK, TABLE, HASH_VALUE, NODE)                       \
  do {                                                                         \
    hash_cell_t *cell3333;                                                     \
    TYPE *struct3333;                                                          \
    const uint64_t hash_value3333 = HASH_VALUE;                                \
                                                                               \
    (NODE)->LINK = NULL;                                                       \
                                                                               \
    cell3333 =                                                                 \
        hash_get_nth_cell(TABLE, hash_calc_cell_id(hash_value3333, TABLE));    \
                                                                               \
    if (cell3333->node == NULL) {                                              \
      cell3333->node = NODE;                                                   \
    } else {                                                                   \
      struct3333 = (TYPE *)cell3333->node;                                     \
                                                                               \
      while (struct3333->LINK != NULL) {                                       \
        struct3333 = (TYPE *)struct3333->LINK;                                 \
      }                                                                        \
                                                                               \
      struct3333->LINK = NODE;                                                 \
    }                                                                          \
  } while (0)

#define HASH_DELETE(TYPE, LINK, TABLE, HASH_VALUE, NODE)                       \
  do {                                                                         \
    hash_cell_t *cell3333;                                                     \
    TYPE *struct3333;                                                          \
    const uint64_t hash_value3333 = HASH_VALUE;                                \
                                                                               \
    cell3333 =                                                                 \
        hash_get_nth_cell(TABLE, hash_calc_cell_id(hash_value3333, TABLE));    \
                                                                               \
    if (cell3333->node == NODE) {                                              \
      cell3333->node = NODE->LINK;                                             \
    } else {                                                                   \
      struct3333 = (TYPE *)cell3333->node;                                     \
                                                                               \
      while (struct3333->LINK != NODE) {                                       \
        struct3333 = (TYPE *)struct3333->LINK;                                 \
      }                                                                        \
                                                                               \
      struct3333->LINK = NODE->LINK;                                           \
    }                                                                          \
  } while (0)

static inline hash_cell_t *hash_get_nth_cell(hash_table_t *table, size_t n) {
  return table->cells + n;
}

static inline uint64_t hash_calc_cell_id(uint64_t hash_value,
                                         hash_table_t *table) {
  return hash_value % table->n_cells;
}

static inline void *hash_get_first(hash_table_t *table, size_t cell_id) {
  return hash_get_nth_cell(table, cell_id)->node;
}

#define HASH_GET_NEXT(LINK, NODE) ((NODE)->LINK)

#define HASH_SEARCH(LINK, TABLE, HASH_VALUE, TYPE, NODE, COND)                 \
  {                                                                            \
    const uint64_t hash_value3333 = HASH_VALUE;                                \
                                                                               \
    (NODE) =                                                                   \
        (TYPE)hash_get_first(TABLE, hash_calc_cell_id(hash_value3333, TABLE)); \
                                                                               \
    while ((NODE) != NULL) {                                                   \
      if (COND) {                                                              \
        break;                                                                 \
      } else {                                                                 \
        (NODE) = (TYPE)HASH_GET_NEXT(LINK, NODE);                              \
      }                                                                        \
    }                                                                          \
  }

#define HASH_SEARCH_ALL(LINK, TABLE, TYPE, NODE, COND)                         \
  do {                                                                         \
    size_t i3333;                                                              \
                                                                               \
    for (i3333 = (TABLE)->n_cells; i3333--;) {                                 \
      (NODE) = (TYPE)hash_get_first(TABLE, i3333);                             \
                                                                               \
      while ((NODE) != NULL) {                                                 \
        if (COND) {                                                            \
          break;                                                               \
        }                                                                      \
                                                                               \
        (NODE) = (TYPE)HASH_GET_NEXT(LINK, NODE);                              \
      }                                                                        \
                                                                               \
      if ((NODE) != NULL) {                                                    \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  } while (0)

static inline void hash_table_clear(hash_table_t *table) {
  memset(table->cells, 0x0, table->n_cells * sizeof(hash_cell_t));
  if (table->heap)
    mem_heap_empty(table->heap);
}

static inline hash_table_t *hash_create(size_t n) {
  uint64_t prime;
  hash_table_t *table;

  prime = find_prime(n);

  table = (hash_table_t *)mem_alloc(sizeof(hash_table_t));
  table->cells = (hash_cell_t *)mem_alloc(sizeof(hash_cell_t) * prime);
  table->n_cells = prime;
  table->heap = NULL;

  hash_table_clear(table);

  return table;
}

static inline void hash_table_free(hash_table_t *table) {
  if (table->heap)
    mem_heap_free(table->heap);
  mem_free(table->cells);
  mem_free(table);
}

typedef struct hash_8b_t {
  uint64_t val;
  hash_node_t link;
} hash_8b_t;

static inline void hash_insert_8b(hash_table_t *table, uint64_t val) {
  hash_8b_t *cell;
  if (unlikely(table->heap == NULL))
    table->heap = mem_heap_create(0);
  cell = (hash_8b_t *)mem_heap_alloc(table->heap, sizeof(hash_8b_t));
  cell->val = val;
  HASH_INSERT(hash_8b_t, link, table, val, cell);
}

static inline hash_8b_t *hash_search_8b(hash_table_t *table, uint64_t val) {
  hash_8b_t *cell;
  HASH_SEARCH(link, table, val, hash_8b_t *, cell, cell->val == val);
  return cell;
}

static inline void hash_delete_8b(hash_table_t *table, hash_8b_t *cell) {
  HASH_DELETE(hash_8b_t, link, table, cell->val, cell);
  mem_heap_free_top(table->heap, sizeof(hash_8b_t));
}

#endif