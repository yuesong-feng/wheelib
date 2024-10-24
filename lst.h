/*

  lst.h

  list utilities, from MySQL 8.4.0 InnoBase

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05

  Version 1.1 2024/09/09 add LIST_FOREACH

  Version 1.2 2024/09/10 add lst_t, lst_node_t and relative operation

  Version 1.2 2024/10/24 sync MySQL 9.1.0
*/
#ifndef LST_H
#define LST_H
#include <stddef.h> // for size_t, NULL

#define LIST(TYPE) \
  struct {         \
    size_t count;  \
    TYPE *first;   \
    TYPE *last;    \
  }

#define LIST_NODE(TYPE) \
  struct {              \
    TYPE *prev;         \
    TYPE *next;         \
  }

#define LIST_INIT(LIST)  \
  {                      \
    (LIST).count = 0;    \
    (LIST).first = NULL; \
    (LIST).last = NULL;  \
  }

#define LIST_ADD_FIRST(LINK, LIST, NODE)    \
  {                                         \
    ((NODE)->LINK).prev = NULL;             \
    ((NODE)->LINK).next = (LIST).first;     \
                                            \
    if ((LIST).first != NULL)               \
      (((LIST).first)->LINK).prev = (NODE); \
                                            \
    (LIST).first = (NODE);                  \
                                            \
    if ((LIST).last == NULL)                \
      (LIST).last = (NODE);                 \
                                            \
    ++((LIST).count);                       \
  }

#define LIST_ADD_LAST(LINK, LIST, NODE)    \
  {                                        \
    ((NODE)->LINK).next = NULL;            \
    ((NODE)->LINK).prev = (LIST).last;     \
                                           \
    if ((LIST).last != NULL)               \
      (((LIST).last)->LINK).next = (NODE); \
                                           \
    (LIST).last = (NODE);                  \
                                           \
    if ((LIST).first == NULL)              \
      (LIST).first = (NODE);               \
                                           \
    ++((LIST).count);                      \
  }

#define LIST_INSERT_AFTER(LINK, LIST, NODE1, NODE2)  \
  {                                                  \
    ((NODE2)->LINK).prev = (NODE1);                  \
    ((NODE2)->LINK).next = ((NODE1)->LINK).next;     \
                                                     \
    if (((NODE2)->LINK).next != NULL)                \
      ((((NODE2)->LINK).next)->LINK).prev = (NODE2); \
    else                                             \
      (LIST).last = (NODE2);                         \
                                                     \
    ((NODE1)->LINK).next = (NODE2);                  \
                                                     \
    ++(LIST).count;                                  \
  }

#define LIST_REMOVE(LINK, LIST, NODE)                           \
  {                                                             \
    if (((NODE)->LINK).next != NULL)                            \
      ((((NODE)->LINK).next)->LINK).prev = ((NODE)->LINK).prev; \
    else                                                        \
      (LIST).last = ((NODE)->LINK).prev;                        \
                                                                \
    if (((NODE)->LINK).prev != NULL)                            \
      ((((NODE)->LINK).prev)->LINK).next = ((NODE)->LINK).next; \
    else                                                        \
      (LIST).first = ((NODE)->LINK).next;                       \
                                                                \
    ((NODE)->LINK).next = NULL;                                 \
    ((NODE)->LINK).prev = NULL;                                 \
                                                                \
    --(LIST).count;                                             \
  }

#define LIST_GET_NEXT(LINK, NODE) (((NODE)->LINK).next)

#define LIST_GET_PREV(LINK, NODE) (((NODE)->LINK).prev)

#define LIST_GET_LEN(LIST) (LIST).count

#define LIST_GET_FIRST(LIST) (LIST).first

#define LIST_GET_LAST(LIST) (LIST).last

#define LIST_FOREACH(LINK, NODE, LIST) \
  for ((NODE) = LIST_GET_FIRST(LIST); (NODE) != NULL; (NODE) = LIST_GET_NEXT(LINK, NODE))

#endif