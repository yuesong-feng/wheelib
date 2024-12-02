/*

  lst.h

  list utilities, from MySQL 8.4.0 InnoBase

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05

  Version 1.1 2024/09/09 add LIST_FOREACH

  Version 1.2 2024/09/10 add lst_t, lst_node_t and relative operation

  Version 1.2 2024/10/24 sync MySQL 9.1.0

  Version 1.3 2024/12/02 stable release
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

/** Initializes a two-way list.
 @param LIST the list */
#define LIST_INIT(LIST)  \
  {                      \
    (LIST).count = 0;    \
    (LIST).first = NULL; \
    (LIST).last = NULL;  \
  }

/** Adds the node as the first node in a two-way linked list.
 @param LINK field name of list link in NODE struct
 @param LIST the list (not a pointer to it)
 @param NODE the node to add */
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

/** Adds the node as the last node in a two-way linked list.
 @param LINK field name of list link in NODE struct
 @param LIST the list (not a pointer to it)
 @param NODE the node to add */
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

/** Inserts a NODE2 after NODE1 in a list.
 @param LINK field name of list link in NODE1/2 struct
 @param LIST the list (not a pointer to it)
 @param NODE1 node after which NODE2 is inserted
 @param NODE2 node being inserted after NODE1 */
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

/** Removes a node from a two-way linked list.
 @param LINK field name of list link in NODE struct
 @param LIST the list (not a pointer to it)
 @param NODE node to be removed from the list */
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

/** Gets the next node in a two-way list.
 @param LINK field name of list link in NODE struct
 @param NODE pointer to a node
 @return the successor of NODE in LINK, or NULL */
#define LIST_GET_NEXT(LINK, NODE) (((NODE)->LINK).next)

/** Gets the previous node in a two-way list.
 @param LINK field name of list link in NODE struct
 @param NODE pointer to a node
 @return the predecessor of NODE in LINK, or NULL */
#define LIST_GET_PREV(LINK, NODE) (((NODE)->LINK).prev)

/** Gets the number of nodes in a two-way list.
 @param LIST the list (not a pointer to it).
 @return the number of nodes in the list */
#define LIST_GET_LEN(LIST) (LIST).count

/** Gets the first node in a two-way list.
 @param LIST the list (not a pointer to it)
 @return first node, or NULL if the list is empty */
#define LIST_GET_FIRST(LIST) (LIST).first

/** Gets the last node in a two-way list.
 @param LIST the list (not a pointer to it)
 @return last node, or NULL if the list is empty */
#define LIST_GET_LAST(LIST) (LIST).last

/** Traverses a two-way list.
 @param LINK field name of list link in NODE struct
 @param NODE the list node used for traversal
 @param LIST the list (not a pointer to it) */
#define LIST_FOREACH(LINK, NODE, LIST) \
  for ((NODE) = LIST_GET_FIRST(LIST); (NODE) != NULL; (NODE) = LIST_GET_NEXT(LINK, NODE))

#endif