#ifndef LIST_H
#define LIST_H
#include <stdbool.h>
#include <stddef.h>

typedef struct list_node_t list_node_t;
struct list_node_t {
    void *data;
    list_node_t *next;
    list_node_t *prev;
};

typedef struct list_t list_t;
struct list_t {
    list_node_t *node;
};

void list_init(list_t *list);

void list_init1(list_t *list, size_t count);

void list_init2(list_t *list, size_t count, void *value);

void list_destroy(list_t *list);

void list_assign(list_t *list, size_t count, void *value);

void list_assign2(list_t *list, list_node_t *first, list_node_t *last);

void *list_front(list_t *list);

void *list_back(list_t *list);

list_node_t *list_begin(list_t *list);

list_node_t *list_end(list_t *list);

bool list_empty(list_t *list);

size_t list_size(list_t *list);

size_t list_max_size(list_t *list);

void list_clear(list_t *list);

list_node_t *list_insert(list_t *list, list_node_t *pos, void *value);

list_node_t *list_insert1(list_t *list, list_node_t *pos, size_t count, void *value);

list_node_t *list_insert2(list_t *list, list_node_t *pos, list_node_t *first, list_node_t *last);

list_node_t *list_erase(list_t *list, list_node_t *pos);

list_node_t *list_erase2(list_t *list, list_node_t *first, list_node_t *last);

void list_push_back(list_t *list, void *value);

void list_pop_back(list_t *list);

void list_push_front(list_t *list, void *value);

void list_pop_front(list_t *list);

void list_resize(list_t *list, size_t count);

void list_resize1(list_t *list, size_t count, void *value);

void list_swap(list_t *list, list_t *other);

typedef bool (*Compare)(const void *, const void *);
void list_merge(list_t *list, list_t *other);

void list_merge1(list_t *list, list_t *other, Compare comp);

void list_splice(list_t *list, list_node_t *pos, list_t *other);

void list_splice1(list_t *list, list_node_t *pos, list_t *other, list_node_t *it);

void list_splice2(list_t *list, list_node_t *pos, list_t *other, list_node_t *first, list_node_t *last);

size_t list_remove(list_t *list, void *value);

typedef bool (*UnaryPred)(const void *);
size_t list_remove_if(list_t *list, UnaryPred p);

void list_reverse(list_t *list);

typedef bool (*BinaryPred)(const void *, const void *);
size_t list_unique(list_t *list);

size_t list_unique1(list_t *list, BinaryPred p);

void list_sort(list_t *list);

void list_sort1(list_t *list, Compare comp);

#endif