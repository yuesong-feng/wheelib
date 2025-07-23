/**
 * @file list.c
 * @date 2025-01-28
 * @author yuesong-feng
 */
#include "list.h"
#include <stdlib.h>
#include <assert.h>

list_t *list_create() {
  list_t *list = malloc(sizeof(list_t));
  list->first = NULL;
  list->last = NULL;
  return list;
}

void list_free(list_t *list) {
  free(list);
}

list_node_t *list_add_first(list_t *list, void *data) {
  return list_add_after(list, list_get_first(list), data);
}

list_node_t *list_add_last(list_t *list, void *data) {
  return list_add_after(list, list_get_last(list), data);
}

list_node_t *list_add_after(list_t *list, list_node_t *prev_node, void *data) {
  list_node_t *node = malloc(sizeof(list_node_t));
  node->data = data;
  if (!list->first) {
    // Empty list
    assert(!prev_node);
    node->prev = NULL;
    node->next = NULL;
    list->first = node;
    list->last = node;
  } else if (!prev_node) {
    // Start of list
    node->prev = NULL;
    node->next = list->first;
    list->first->prev = node;
    list->first = node;
  } else {
    // Middle or end of list
    node->prev = prev_node;
    node->next = prev_node->next;
    prev_node->next = node;
    if (node->next) {
      node->next->prev = node;
    } else {
      list->last = node;
    }
  }
  return node;
}

void list_remove(list_t *list, list_node_t *node) {
  if (node->prev) {
    node->prev->next = node->next;
  } else {
    // First item in list
    assert(list->first == node);
    list->first = node->next;
  }

  if (node->next) {
    node->next->prev = node->prev;
  } else {
    // Last item in list
    assert(list->last == node);
    list->last = node->prev;
  }

  node->prev = node->next = NULL;
  free(node);
}

list_node_t *list_get_first(list_t *list) {
  return list->first;
}

list_node_t *list_get_last(list_t *list) {
  return list->last;
}

bool list_is_empty(const list_t *list) {
  return !(list->first || list->last);
}