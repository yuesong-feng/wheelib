/*

  mem.h

  memory utilities, from MySQL 8.4.0 InnoBase
  alloc memory fail will cause system halt immediately,
  so it's unnecessary for upper invocation to consider NULL-return

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05

  Version 1.1 2024/09/24 add mem_strdup mem_strdupl

  Version 1.2 2024/10/24 change to MySQL 9.1.0
*/
#ifndef MEM_H
#define MEM_H
#include "byte.h"   // for byte
#include "calc.h"   // for calc_align
#include "lst.h"    // for list
#include <string.h> // for memcpy

typedef struct mem_block_t mem_block_t;
struct mem_block_t {
  LIST(mem_block_t) base;
  LIST_NODE(mem_block_t) link;
  size_t len;
  size_t total_size;
  size_t free;
  size_t start;
  #ifdef WLIB_DEBUG
  uint64_t magic_n;
  char file_name[16];
  int line;
  #endif
};

typedef mem_block_t mem_heap_t;

#ifdef WLIB_DEBUG
  #define MEM_NO_MANS_LAND 16
#else
  #define MEM_NO_MANS_LAND 0
#endif
#define MEM_NO_MANS_LAND_BEFORE_BYTE 0xCE
#define MEM_NO_MANS_LAND_AFTER_BYTE 0xDF

#define MEM_BLOCK_START_SIZE 64
#define MEM_BLOCK_STANDARD_SIZE 8000
#define MEM_ALIGNMENT 8
#define MEM_BLOCK_HEADER_SIZE calc_align(sizeof(mem_block_t), MEM_ALIGNMENT)

#define MEM_BLOCK_MAGIC_N 0x445566778899AABBULL
#define MEM_FREED_BLOCK_MAGIC_N 0xBBAA998877665544ULL

static inline uint64_t MEM_SPACE_NEEDED(uint64_t N) {
  return calc_align(N + 2 * MEM_NO_MANS_LAND, MEM_ALIGNMENT);
}

/** Checks that an object is a memory heap (or a block of it)
@param[in]      block   Memory heap to check */
static inline void mem_block_validate(const mem_block_t *block) {
  astd(block->magic_n == MEM_BLOCK_MAGIC_N);
}

/** Validates the contents of a memory heap.
Checks a memory heap for consistency, prints the contents if any error
is detected. A fatal error is logged if an error is detected.
@param[in]      heap    Memory heap to validate. */
static inline void mem_heap_validate(const mem_heap_t *heap) {
  size_t size = 0;

  for (const mem_block_t *block = heap; block != NULL; block = LIST_GET_NEXT(link, block)) {
    mem_block_validate(block);

    size += block->len;
  }

  ut_ad(size == heap->total_size);
}

/** Creates a memory heap block where data can be allocated.
 @return own: memory heap block, NULL if did not succeed (only possible
 for MEM_HEAP_BTR_SEARCH type heaps)
 @param[in] heap memory heap or NULL if first block should be created
 @param[in] n number od bytes needed for user data
 @param[in] file_name file name where created
 @param[in] line line where created
 @param[in] type type of heap MEM_HEAP_DYNAMIC or MEM_HEAP_BUFFER */
static inline mem_block_t *mem_heap_create_block(mem_heap_t *heap, size_t n, IF_DEBUG(const char *filename, int line)) {
  mem_block_t *block;
  size_t len;

  if (heap != NULL) {
    mem_block_validate(heap);
    #ifdef WLIB_DEBUG
    mem_heap_validate(heap);
    #endif
  }

  len = MEM_BLOCK_HEADER_SIZE + MEM_SPACE_NEEDED(n);

  block = malloc(len);
  ast(block);

  block->magic_n = MEM_BLOCK_MAGIC_N;
  block->len = len;
  block->start = MEM_BLOCK_HEADER_SIZE;
  block->free = MEM_BLOCK_HEADER_SIZE;

  if (unlikely(heap == NULL))
  {

    /* This is the first block of the heap. The field
    total_size should be initialized here */
    block->total_size = len;
  }
  else
  {
    /* Not the first allocation for the heap. This block's
    total_length field should be set to undefined and never
    actually used. */
    heap->total_size += len;
  }

  astd(MEM_BLOCK_HEADER_SIZE < len);

  return block;
}

static inline mem_heap_t *mem_heap_create(size_t size, const char* filename, int line) {
  mem_block_t *block;

  if (!size)
    size = MEM_BLOCK_START_SIZE;

  block = mem_heap_create_block(NULL, size, IF_DEBUG(filename, line));

  LIST_INIT(block->base);

  LIST_ADD_FIRST(link, block->base, block);

  return block;
}

#ifdef WLIB_DEBUG
static inline void validate_no_mans_land(byte *no_mans_land_begin,
                                         byte mem_no_mans_land_byte) {
  for (byte *no_mans_land_it = no_mans_land_begin;
       no_mans_land_it < no_mans_land_begin + MEM_NO_MANS_LAND;
       ++no_mans_land_it) {
    ast(*no_mans_land_it == mem_no_mans_land_byte);
  }
}
#endif

/** Frees a block from a memory heap. */
static inline void mem_heap_block_free(mem_heap_t *heap, mem_block_t *block) {
  mem_block_validate(block);

  LIST_REMOVE(link, heap->base, block);

  astd(heap->total_size >= block->len);
  heap->total_size -= block->len;

  block->magic_n = MEM_FREED_BLOCK_MAGIC_N;

#ifdef WLIB_DEBUG
  if (block->start != block->free) {
    validate_no_mans_land((byte *)block + block->start,
                          MEM_NO_MANS_LAND_BEFORE_BYTE);
    validate_no_mans_land(
        (byte *)block + block->free - MEM_NO_MANS_LAND,
        MEM_NO_MANS_LAND_AFTER_BYTE);
  }
#endif

  free(block);
}

/** Frees the space occupied by a memory heap.
NOTE: Use the corresponding macro instead of this function.
@param[in]      heap    Heap to be freed */
static inline void mem_heap_free(mem_heap_t *heap) {
  mem_block_t *block;
  mem_block_t *prev_block;

  ifdbg(mem_block_validate(heap));

  block = LIST_GET_LAST(heap->base);

  while (block != NULL) {
    /* Store the contents of info before freeing current block
    (it is erased in freeing) */

    prev_block = LIST_GET_PREV(link, block);

    mem_heap_block_free(heap, block);

    block = prev_block;
  }
}

/** Adds a new block to a memory heap.
 @param[in]     heap    memory heap
 @param[in]     n       number of bytes needed
 @return created block, NULL if did not succeed (only possible for
 MEM_HEAP_BTR_SEARCH type heaps) */
static inline mem_block_t *mem_heap_add_block(mem_heap_t *heap, size_t n) {
  mem_block_t *block;
  mem_block_t *new_block;
  size_t new_size;

  ifdbg(mem_block_validate(heap));

  block = LIST_GET_LAST(heap->base);

  /* We have to allocate a new block. The size is always at least
  doubled until the standard size is reached. After that the size
  stays the same, except in cases where the caller needs more space. */

  new_size = 2 * block->len;

  if (new_size > MEM_BLOCK_STANDARD_SIZE)
    new_size = MEM_BLOCK_STANDARD_SIZE;

  if (new_size < n)
    new_size = n;

  new_block = mem_heap_create_block(heap, new_size, IF_DEBUG(heap->file_name, heap->line));
  if (new_block == NULL)
    return NULL;

  LIST_INSERT_AFTER(link, heap->base, block, new_block);

  return new_block;
}

/** Allocates n bytes of memory from a memory heap.
@param[in]      heap    memory heap
@param[in]      n       number of bytes; if the heap is allowed to grow into
the buffer pool, this must be <= MEM_MAX_ALLOC_IN_BUF
@return allocated storage, NULL if did not succeed (only possible for
MEM_HEAP_BTR_SEARCH type heaps) */
static inline void *mem_heap_alloc(mem_heap_t *heap, size_t n) {
  mem_block_t *block;
  byte *buf;
  size_t free;

  ifdbg(mem_block_validate(heap));

  block = LIST_GET_LAST(heap->base);

  /* Check if there is enough space in block. If not, create a new
  block to the heap */

  if (block->len < block->free + MEM_SPACE_NEEDED(n)) {
    block = mem_heap_add_block(heap, n);
    if (block == NULL)
      return NULL;
  }

  free = block->free;

  buf = (byte *)block + free + MEM_NO_MANS_LAND;

  block->free = free + MEM_SPACE_NEEDED(n);

#ifdef WLIB_DEBUG
  byte *start_no_mans_land = buf - MEM_NO_MANS_LAND;
  byte *end_no_mans_land =
      start_no_mans_land + MEM_SPACE_NEEDED(n) - MEM_NO_MANS_LAND;
  memset(start_no_mans_land, MEM_NO_MANS_LAND_BEFORE_BYTE, MEM_NO_MANS_LAND);

  memset(end_no_mans_land, MEM_NO_MANS_LAND_AFTER_BYTE, MEM_NO_MANS_LAND);
#endif

  return buf;
}

/** Allocates and zero-fills n bytes of memory from a memory heap.
@param[in]      heap    memory heap
@param[in]      n       number of bytes; if the heap is allowed to grow into
the buffer pool, this must be <= MEM_MAX_ALLOC_IN_BUF
@return allocated, zero-filled storage */
static inline void *mem_heap_zalloc(mem_heap_t *heap, size_t n) {
  astd(heap);
  return memset(mem_heap_alloc(heap, n), 0, n);
}

/** Returns a pointer to the heap top.
@param[in]      heap    memory heap
@return pointer to the heap top */
static inline byte *mem_heap_get_heap_top(mem_heap_t *heap) {
  mem_block_t *block;
  byte *buf;

  ifdbg(mem_block_validate(heap));

  block = LIST_GET_LAST(heap->base);

  buf = (byte *)block + block->free;

  return buf;
}

/** Frees the space in a memory heap exceeding the pointer given.
The pointer must have been acquired from mem_heap_get_heap_top.
The first memory block of the heap is not freed.
@param[in]      heap            heap from which to free
@param[in]      old_top         pointer to old top of heap */
static inline void mem_heap_free_heap_top(mem_heap_t *heap, byte *old_top) {
  mem_block_t *block;
  mem_block_t *prev_block;

  ifdbg(mem_block_validate(heap));

  block = LIST_GET_LAST(heap->base);

  while (block != NULL) {
    if (((byte *)block + block->free >= old_top) && ((byte *)block <= old_top))
    {
      /* Found the right block */
      break;
    }

    /* Store prev_block value before freeing the current block
    (the current block will be erased in freeing) */

    prev_block = LIST_GET_PREV(link, block);

    mem_heap_block_free(heap, block);

    block = prev_block;
  }

  astd(block);

  if (old_top == (byte *)block + block->free)
    return;

#ifdef WLIB_DEBUG
  validate_no_mans_land(old_top, MEM_NO_MANS_LAND_BEFORE_BYTE);
  validate_no_mans_land(
      (byte *)block + block->free - MEM_NO_MANS_LAND,
      MEM_NO_MANS_LAND_AFTER_BYTE);
#endif

  block->free = old_top - (byte *)block;

  astd(block->start <= block->free);

  /* If free == start, we may free the block if it is not the first one */
  if ((heap != block) && (block->free == block->start)) {
    mem_heap_block_free(heap, block);
  }
}

/** Empties a memory heap.
The first memory block of the heap is not freed.
@param[in]      heap    heap to empty */
static inline void mem_heap_empty(mem_heap_t *heap) {
  mem_heap_free_heap_top(heap, (byte *)heap + heap->start);
}

// TODO: n is bigger than block size?
static inline void *mem_heap_get_top(mem_heap_t *heap, size_t n) {
  mem_block_t *block;
  byte *buf;

  block = LIST_GET_LAST(heap->base);

  buf = (byte *)block + block->free - MEM_SPACE_NEEDED(n);

  return (void *)buf;
}

/** Checks if a given chunk of memory is the topmost element stored in the
heap. If this is the case, then calling mem_heap_free_top() would free
that element from the heap.
@param[in]      heap    memory heap
@param[in]      buf     presumed topmost element
@param[in]      buf_sz  size of buf in bytes
@return true if topmost */
static inline bool mem_heap_is_top(mem_heap_t *heap, const void *buf, size_t buf_sz) {
  const byte *first_free_byte;
  const byte *presumed_start_of_buf;

  ifdbg(mem_block_validate(heap));

  first_free_byte = mem_heap_get_heap_top(heap);

  presumed_start_of_buf = first_free_byte - MEM_SPACE_NEEDED(buf_sz) + MEM_NO_MANS_LAND;

  return presumed_start_of_buf == buf;
}

/** Frees the topmost element in a memory heap. The size of the element must be given. */
static inline void mem_heap_free_top(mem_heap_t *heap, size_t n) {
  mem_block_t *block;

  ifdbg(mem_block_validate(heap));

  block = LIST_GET_LAST(heap->base);

  block->free = block->free - MEM_SPACE_NEEDED(n);

#ifdef WLIB_DEBUG
  validate_no_mans_land((byte *)block + mem_block_get_free(block),
                        MEM_NO_MANS_LAND_BEFORE_BYTE);
  validate_no_mans_land((byte *)block + mem_block_get_free(block) +
                            MEM_SPACE_NEEDED(n) - MEM_NO_MANS_LAND,
                        MEM_NO_MANS_LAND_AFTER_BYTE);
#endif

  /* If free == start, we may free the block if it is not the first one */
  if ((heap != block) && (block->free == block->start)) {
    mem_heap_block_free(heap, block);
  }
}

/** Duplicate a block of data, allocated from a memory heap.
 @return own: a copy of the data */
static inline void *mem_heap_dup(mem_heap_t *heap, const void *data, size_t len) {
  return memcpy(mem_heap_alloc(heap, len), data, len);
}

/** Duplicates a NUL-terminated string, allocated from a memory heap.
@param[in]      heap    memory heap where string is allocated
@param[in]      str     string to be copied
@return own: a copy of the string */
static inline char *mem_heap_strdup(mem_heap_t *heap, const char *str) {
  return (char *)mem_heap_dup(heap, str, strlen(str) + 1);
}

/** Makes a NUL-terminated copy of a nonterminated string,
 allocated from a memory heap.
 @return own: a copy of the string */
static inline char *mem_heap_strdupl(mem_heap_t *heap, const char *str, size_t len) {
  char *s = (char *)mem_heap_alloc(heap, len + 1);
  s[len] = 0;
  if (len > 0)
    memcpy(s, str, len);
  return s;
}

/** Concatenate two strings and return the result, using a memory heap.
 @return own: the result */
static inline char *mem_heap_strcat(mem_heap_t *heap, const char *s1, const char *s2) {
  char *s;
  size_t s1_len = strlen(s1);
  size_t s2_len = strlen(s2);

  s = (char *)mem_heap_alloc(heap, s1_len + s2_len + 1);

  memcpy(s, s1, s1_len);
  memcpy(s + s1_len, s2, s2_len);

  s[s1_len + s2_len] = '\0';

  return s;
}

static inline void *mem_alloc(size_t n) {
  mem_heap_t *heap;
  void *buf;

  heap = mem_heap_create(n);

  buf = mem_heap_alloc(heap, n);

  return buf;
}

static inline void mem_free(void *ptr) {
  mem_heap_t *heap;

  heap = (mem_heap_t *)((byte *)ptr - MEM_BLOCK_HEADER_SIZE);

  mem_heap_free(heap);
}

/** Duplicates a NUL-terminated string.
 @return own: a copy of the string, must be deallocated with ut_free */
static inline char *mem_strdup(const char *str) {
  size_t len = strlen(str) + 1;
  return (char *)memcpy(mem_alloc(len), str, len);
}

/** Makes a NUL-terminated copy of a nonterminated string.
 @return own: a copy of the string, must be deallocated with ut_free */
static inline char *mem_strdupl(const char *str, size_t len) {
  char *s = (char *)mem_alloc(len + 1);
  s[len] = 0;
  if (len > 0)
    memcpy(s, str, len);
  return s;
}

#endif