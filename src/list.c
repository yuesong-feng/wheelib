#include "list.h"
#include <stdlib.h>

list_t *list_create(alloc_t *alloc) {
  list_t *list;

  list = alloc->malloc(alloc, sizeof(list_t));

  list->alloc = alloc;
  list->first = NULL;
  list->last = NULL;

  return list;
}

void list_free(list_t *list) {
  list->alloc->free(list->alloc, list);
}

list_node_t *list_get_first(list_t *list) {
  return list->first;
}

list_node_t *list_get_last(list_t *list) {
  return list->last;
}

bool list_is_empty(list_t *list) {
  return !(list->first || list->last);
}

list_node_t *list_add_first(list_t *list, void *data) {
  return list_add_after(list, list_get_first(list), data);
}

list_node_t *list_add_last(list_t *list, void *data) {
  return list_add_after(list, list_get_last(list), data);
}

list_node_t *list_add_after(list_t *list, list_node_t *prev, void *data) {
  list_node_t *node;

  node = list->alloc->malloc(list->alloc, sizeof(list_node_t));

  node->data = data;

  if (list->first == NULL) {
    node->prev = NULL;
    node->next = NULL;
    list->first = node;
    list->last = node;
  } else if (prev == NULL) {
    node->prev = NULL;
    node->next = list->first;
    list->first->prev = node;
    list->first = node;
  } else {
    node->prev = prev;
    node->next = prev->next;
    prev->next = node;
    if (node->next != NULL)
      node->next->prev = node;
    else
      list->last = node;
  }

  return node;
}

void list_remove(list_t *list, list_node_t *node) {
  if (node->prev)
    node->prev->next = node->next;
  else
    list->first = node->next;

  if (node->next)
    node->next->prev = node->prev;
  else
    list->last = node->prev;

  node->prev = node->next = NULL;
}