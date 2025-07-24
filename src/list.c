#include "list.h"
#include <stdlib.h>

void list_init(list_t *list) {
    list->node = malloc(sizeof(list_node_t));
    list->node->data = NULL;
    list->node->next = list->node;
    list->node->prev = list->node;
}

void list_init1(list_t *list, size_t count) {
    list_init(list);
    list_insert1(list, list_begin(list), count, NULL);
}

void list_init2(list_t *list, size_t count, void *value) {
    list_init(list);
    list_insert1(list, list_begin(list), count, value);
}

void list_destroy(list_t *list) {
    list_clear(list);
    free(list->node);
}

void list_assign(list_t *list, size_t count, void *value) {
    list_node_t *i = list_begin(list);
    for ( ; i != list_end(list) && count > 0; i = i->next, --count)
        i->data = value;
    if (count > 0)
        list_insert1(list, list_end(list), count, value);
    else
        list_erase2(list, i, list_end(list));
}

void list_assign2(list_t *list, list_node_t *first, list_node_t *last) {
    list_node_t *begin = list_begin(list);
    list_node_t *end = list_end(list);
    for ( ; begin != end && first != last; begin = begin->next, first = first->next)
        begin->data = first->data;
    if (first == last)
        list_erase2(list, begin, end);
    else
        list_insert2(list, end, first, last);
}

void *list_front(list_t *list) {
    return list_begin(list)->data;
}

void *list_back(list_t *list) {
    return list_end(list)->prev->data;
}

list_node_t *list_begin(list_t *list) {
    return list->node->next;
}

list_node_t *list_end(list_t *list) {
    return list->node;
}

bool list_empty(list_t *list) {
    return list->node->next == list->node;
}

size_t list_size(list_t *list) {
    size_t n = 0;
    list_node_t *first = list_begin(list);
    list_node_t *last = list_end(list);
    while (first != last) {
        first = first->next;
        ++n;
    }
    return n;
}

size_t list_max_size(list_t *list) {
    return (size_t)(-1);
}

void list_clear(list_t *list) {
    list_node_t *cur = list->node->next;
    while (cur != list->node) {
        list_node_t *tmp = cur;
        cur = cur->next;
        // Destroy tmp->data
        free(tmp);
    }
    list->node->next = list->node;
    list->node->prev = list->node;
}

list_node_t *list_insert(list_t *list, list_node_t *pos, void *value) {
    list_node_t *tmp = malloc(sizeof(list_node_t));
    tmp->data = value;
    tmp->next = pos;
    tmp->prev = pos->prev;
    pos->prev->next = tmp;
    pos->prev = tmp;
    return tmp;
}

list_node_t *list_insert1(list_t *list, list_node_t *pos, size_t count, void *value) {
    for ( ; count > 0; --count)
        list_insert(list, pos, value);
}

list_node_t *list_insert2(list_t *list, list_node_t *pos, list_node_t *first, list_node_t *last) {
    for ( ; first != last; first = first->next)
        list_insert(list, pos, first->data);
}

list_node_t *list_erase(list_t *list, list_node_t *pos) {
    list_node_t *next_node = pos->next;
    list_node_t *prev_node = pos->prev;
    list_node_t *n = pos;
    prev_node->next = next_node;
    next_node->prev = prev_node;
    // Destroy n->data
    free(n);
    return next_node;
}

list_node_t *list_erase2(list_t *list, list_node_t *first, list_node_t *last) {
    while (first != last) {
        list_node_t *tmp = first->next;
        list_erase(list, first);
        first = tmp;
    }
    return last;
}

void list_push_back(list_t *list, void *value) {
    list_insert(list, list_end(list), value);
}

void list_pop_back(list_t *list) {
    list_node_t *tmp = list_end(list);
    tmp = tmp->prev;
    list_erase(list, tmp);
}

void list_push_front(list_t *list, void *value) {
    list_insert(list, list_begin(list), value);
}

void list_pop_front(list_t *list) {
    list_erase(list, list_begin(list));
}

void list_resize(list_t *list, size_t count) {
    list_resize1(list, count, NULL);
}

void list_resize1(list_t *list, size_t count, void *value) {
    list_node_t *i = list_begin(list);
    size_t len = 0;
    for ( ; i != list_end(list) && len < count; i = i->next, ++len)
        ;
    if (len == count)
        list_erase2(list, i, list_end(list));
    else
        list_insert1(list, list_end(list), count - len, value);
}

void list_swap(list_t *list, list_t *other) {
    list_node_t *tmp = list->node;
    list->node = other->node;
    other->node = tmp;
}

void transfer(list_node_t *pos, list_node_t *first, list_node_t *last) {
    if (pos != last) {
        // Remove [first, last) from its old position.
        last->prev->next = pos;
        first->prev->next = last;
        pos->prev->next = first;

        // Splice [first, last) into its new position.
        list_node_t *tmp = pos->prev;
        pos->prev = last->prev;
        last->prev = first->prev;
        first->prev = tmp;
    }
}

void list_merge(list_t *list, list_t *other) {
    list_node_t *first1 = list_begin(list);
    list_node_t *last1 = list_end(list);
    list_node_t *first2 = list_begin(other);
    list_node_t *last2 = list_end(other);
    while (first1 != last1 && first2 != last2)
        if (first2->data < first1->data) {
            list_node_t *next = first2;
            next = next->next;
            transfer(first1, first2, next);
            first2 = next;
        } else 
            first1 = first1->next;
    if (first2 != last2) transfer(last1, first2, last2);
}

void list_merge1(list_t *list, list_t *other, Compare comp) {
    list_node_t *first1 = list_begin(list);
    list_node_t *last1 = list_end(list);
    list_node_t *first2 = list_begin(other);
    list_node_t *last2 = list_end(other);
    while (first1 != last1 && first2 != last2)
        if (comp(first2->data, first1->data)) {
            list_node_t *next = first2;
            next = next->next;
            transfer(first1, first2, next);
            first2 = next;
        } else 
            first1 = first1->next;
    if (first2 != last2) transfer(last1, first2, last2);
}


void list_splice(list_t *list, list_node_t *pos, list_t *other) {
    if (!list_empty(other))
        transfer(pos, list_begin(other), list_end(other));
}

void list_splice1(list_t *list, list_node_t *pos, list_t *other, list_node_t *it) {
    list_node_t *j = it;
    j = j->next;
    if (pos == it || pos == j) return;
    transfer(pos, it, j);
}

void list_splice2(list_t *list, list_node_t *pos, list_t *other, list_node_t *first, list_node_t *last) {
    if (first != last)
        transfer(pos, first, last);
}

size_t list_remove(list_t *list, void *value) {
    list_node_t *first = list_begin(list);
    list_node_t *last = list_end(list);
    size_t ret = 0;
    while (first != last) {
        list_node_t *next = first;
        next = next->next;
        if (first->data == value) {
            list_erase(list, first);
            ret++;
        }
        first = next;
    }
    return ret;
}

typedef bool (*UnaryPred)(const void *);
size_t list_remove_if(list_t *list, UnaryPred p) {
    list_node_t *first = list_begin(list);
    list_node_t *last = list_end(list);
    size_t ret = 0;
    while (first != last) {
        list_node_t *next = first;
        next = next->next;
        if (p(first->data)) {
            list_erase(list, first);
            ret++;
        }
        first = next;
    }
    return ret;
}

void list_reverse(list_t *list) {
    list_node_t *tmp = list->node;
    do {
        list_node_t *tmp2 = tmp->next;
        tmp->next = tmp->prev;
        tmp->prev = tmp2;
        tmp = tmp->prev;    // Old next node is now prev.
    } while (tmp != list->node);
}

size_t list_unique(list_t *list) {
    list_node_t *first = list_begin(list);
    list_node_t *last = list_end(list);
    if (first == last) return 0;
    list_node_t *next = first;
    size_t ret = 0;
    while ((next = next->next) != last) {
        if (first->data == next->data)
        {
            list_erase(list, next);
            ret++;
        }
        else
            first = next;
        next = first;
    }
    return ret;
}

typedef bool (*BinaryPred)(const void *, const void *);
size_t list_unique1(list_t *list, BinaryPred p) {
    list_node_t *first = list_begin(list);
    list_node_t *last = list_end(list);
    if (first == last) return 0;
    list_node_t *next = first;
    size_t ret = 0;
    while ((next = next->next) != last) {
        if (p(first->data, last->data))
        {
            list_erase(list, next);
            ret++;
        }
        else
            first = next;
        next = first;
    }
    return ret;
}

void list_sort(list_t *list) {
    // Do nothing if the list has length 0 or 1.
    if (list->node->next != list->node && list->node->next->next != list->node) {
        list_t carry;
        list_t counter[64];
        list_init(&carry);
        for (int i = 0; i < 64; ++i)
            list_init(&counter[i]);
        int fill = 0;
        while (!list_empty(list)) {
            list_splice1(&carry, list_begin(&carry), list, list_begin(list));
            int i = 0;
            while (i < fill && !list_empty(&counter[i])) {
                list_merge(&counter[i], &carry);
                list_swap(&carry, &counter[i++]);
            }
            list_swap(&carry, &counter[i]);
            if (i == fill) ++fill;
        }

        for (int i = 1; i < fill; ++i)
            list_merge(&counter[i], &counter[i - 1]);
        list_swap(list, &counter[fill - 1]);
    }
}

typedef bool (*Compare)(const void *, const void *);
void list_sort1(list_t *list, Compare comp) {
    // Do nothing if the list has length 0 or 1.
    if (list->node->next != list->node && list->node->next->next != list->node) {
        list_t carry;
        list_t counter[64];
        list_init(&carry);
        for (int i = 0; i < 64; ++i)
            list_init(&counter[i]);
        int fill = 0;
        while (!list_empty(list)) {
            list_splice1(&carry, list_begin(&carry), list, list_begin(list));
            int i = 0;
            while (i < fill && !list_empty(&counter[i])) {
                list_merge1(&counter[i], &carry, comp);
                list_swap(&carry, &counter[i++]);
            }
            list_swap(&carry, &counter[i]);
            if (i == fill) ++fill;
        }

        for (int i = 1; i < fill; ++i)
            list_merge1(&counter[i], &counter[i - 1], comp);
        list_swap(list, &counter[fill - 1]);

        for (int i = 0; i < 64; ++i)
            list_destroy(&counter[i]);
        list_destroy(&carry);
    }
}