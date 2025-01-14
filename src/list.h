#ifndef LIST_H
#define LIST_H
#include "alloc.h"
#include <stdbool.h>

typedef struct list_node_t list_node_t;
struct list_node_t {
  list_node_t *prev;
  list_node_t *next;
  void *data;
};

typedef struct list_t list_t;
struct list_t {
  alloc_t *alloc;
  list_node_t *first;
  list_node_t *last;
};

list_t *list_create(alloc_t *alloc);

void list_free(list_t *list);

list_node_t *list_get_first(list_t *list);

list_node_t *list_get_last(list_t *list);

bool list_is_empty(list_t *list);

list_node_t *list_add_first(list_t *list, void *data);

list_node_t *list_add_last(list_t *list, void *data);

list_node_t *list_add_after(list_t *list, list_node_t *prev, void *data);

void list_remove(list_t *list, list_node_t *node);

#endif