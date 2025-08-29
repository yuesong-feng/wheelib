#ifndef RBTREE_H
#define RBTREE_H
#include <stdbool.h>
#include <stddef.h>
#include "type.h"

typedef bool rbt_color_t;
#define rbt_red false
#define rbt_black true

typedef struct rbt_node_t rbt_node_t;
struct rbt_node_t {
    rbt_color_t color;
    rbt_node_t *parent;
    rbt_node_t *left;
    rbt_node_t *right;
    void *value;
};

typedef struct rbt_t rbt_t;
struct rbt_t {
    rbt_node_t *header;
    size_t count;
    Compare comp;
};




#endif