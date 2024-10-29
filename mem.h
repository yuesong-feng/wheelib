/*

  mem.h

  memory utilities, from MySQL 8.4.0 InnoBase
  alloc memory fail will cause system halt immediately,
  so it's unnecessary for upper invocation to consider NULL-return

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05

  Version 1.1 2024/09/24 add mem_strdup mem_strdupl

  Version 1.2 2024/10/24 change to MySQL 9.1.0

  Version 2.0 2024/10/29 rewrite
*/
#ifndef MEM_H
#define MEM_H

#include "byte.h"   // for byte
#include "calc.h"   // for calc_align
#include "lst.h"    // for list
#include "dbg.h"    // for debug
#include <string.h> // for memcpy
#include <stdarg.h> // for va_list
#include <stdio.h>  // for sprintf

/* -------------------- MEMORY HEAPS ----------------------------- */

/** A block of a memory heap consists of the info structure
followed by an area of memory */
typedef struct mem_block_t mem_block_t;

/** The info structure stored at the beginning of a heap block */
struct mem_block_t {
#ifdef WLIB_DEBUG
  /** Magic number for debugging. */
  uint64_t magic_n;
  /** File name where the mem heap was created. */
  char file_name[16];
  /** Line number where the mem heap was created. */
  int line;
#endif /* WLIB_DEBUG */
  /** This contains pointers to next and prev in the list. The first block
  allocated to the heap is also the first block in this list,
  though it also contains the base node of the list. */
  LIST_NODE(mem_block_t) list;
  /** In the first block of the list this is the base node of the list of
  blocks; in subsequent blocks this is undefined. */
  LIST(mem_block_t) base;
  /** Physical length of this block in bytes. */
  size_t len;
  /** Physical length in bytes of all blocks in the heap. This is defined only
  in the base node and is set to ULINT_UNDEFINED in others. */
  size_t total_size;
  /** Offset in bytes of the first free position for user data in the block. */
  size_t free;
  /** The value of the struct field 'free' at the creation of the block. */
  size_t start;
};

/** A memory heap is a nonempty linear list of memory blocks */
typedef mem_block_t mem_heap_t;

/** The following start size is used for the first block in the memory heap if
the size is not specified, i.e., 0 is given as the parameter in the call of
create. The standard size is the maximum (payload) size of the blocks used for
allocations of small buffers. */
#define MEM_BLOCK_START_SIZE  64

#define MEM_BLOCK_STANDARD_SIZE 1024

/* Before and after any allocated object we will put MEM_NO_MANS_LAND bytes of
some data (different before and after) which is supposed not to be modified by
anyone. This way it would be much easier to determine whether anyone was
writing on not his memory, especially that Valgrind can assure there was no
reads or writes to this memory. */
#ifdef WLIB_DEBUG
#define MEM_NO_MANS_LAND 16
#else
#define MEM_NO_MANS_LAND 0
#endif

/* Byte that we would put before allocated object MEM_NO_MANS_LAND times.*/
#define MEM_NO_MANS_LAND_BEFORE_BYTE 0xCE
/* Byte that we would put after allocated object MEM_NO_MANS_LAND times.*/
#define MEM_NO_MANS_LAND_AFTER_BYTE  0xDF

#define MEM_ALIGNMENT 8

#define MEM_BLOCK_MAGIC_N 0x445566778899AABB
#define MEM_FREED_BLOCK_MAGIC_N 0xBBAA998877665544

/* Header size for a memory heap block */
#define MEM_BLOCK_HEADER_SIZE \
  wl_calc_align(sizeof(mem_block_t), MEM_ALIGNMENT)

/** Space needed when allocating for a user a field of length N.
The space is allocated only in multiples of MEM_ALIGNMENT. In debug mode
contains two areas of no mans lands before and after the buffer requested. */
static inline uint64_t MEM_SPACE_NEEDED(uint64_t N) {
  return wl_calc_align(N + 2 * MEM_NO_MANS_LAND, MEM_ALIGNMENT);
}

#ifdef WLIB_DEBUG
/** Checks that an object is a memory heap (or a block of it)
@param[in]      block   Memory heap to check */
static inline void mem_block_validate(const mem_block_t *block) {
  if (block->magic_n != MEM_BLOCK_MAGIC_N) {
    exit(EXIT_FAILURE);
  }
}

/** Validates the contents of a memory heap.
Checks a memory heap for consistency, prints the contents if any error
is detected. A fatal error is logged if an error is detected.
@param[in]      heap    Memory heap to validate. */
void mem_heap_validate(const mem_heap_t *heap) {
  size_t size = 0;
  const mem_block_t* block;

  for (block = heap; block != NULL; block = LIST_GET_NEXT(list, block)) {
    mem_block_validate(block);

    size += block->len;
  }

  wl_ad(size == heap->total_size);
}
#endif

/** Creates a memory heap block where data can be allocated.
 @return own: memory heap block, NULL if did not succeed
 @param[in] heap memory heap or NULL if first block should be created
 @param[in] n number od bytes needed for user data
 @param[in] file_name file name where created
 @param[in] line line where created
 @param[in] type type of heap MEM_HEAP_DYNAMIC or MEM_HEAP_BUFFER */
static inline mem_block_t *mem_heap_create_block(mem_heap_t *heap, size_t n, const char *file_name, int line) {
  mem_block_t *block;
  size_t len;

  if (heap != NULL) {
    wl_d(mem_block_validate(heap));
    wl_d(mem_heap_validate(heap));
  }

  len = MEM_BLOCK_HEADER_SIZE + MEM_SPACE_NEEDED(n);
  block = (mem_block_t *)(malloc(len));
  wl_a(block);

#ifdef WLIB_DEBUG
{
  size_t src_size = strlen(file_name);
  size_t n = min(src_size, sizeof(block->file_name) - 1);
  memcpy(block->file_name, file_name + src_size - n, n + 1);
}
#endif
  wl_d(block->line = line);

  block->magic_n = MEM_BLOCK_MAGIC_N;
  block->len = len;
  block->start = MEM_BLOCK_HEADER_SIZE;
  wl_ad(MEM_BLOCK_HEADER_SIZE >= block->start && MEM_BLOCK_HEADER_SIZE <= block->len);
  block->free = MEM_BLOCK_HEADER_SIZE;

  if (unlikely(heap == NULL)) {
    /* This is the first block of the heap. The field
    total_size should be initialized here */
    block->total_size = len;
  } else {
    /* Not the first allocation for the heap. This block's
    total_length field should be set to undefined and never
    actually used. */
    wl_d(block->total_size = (~0));

    heap->total_size += len;
  }

  wl_ad((size_t)MEM_BLOCK_HEADER_SIZE < len);

  return (block);
}

/** Creates a memory heap.
A single user buffer of 'size' will fit in the block.
0 creates a default size block.
@param[in]      size            Desired start block size.
@param[in]      loc             Location where created
@return own: memory heap, NULL if did not succeed */
static inline mem_heap_t *mem_heap_create(size_t size, const char* filename, int line) {
  mem_block_t *block;

  if (!size) {
    size = MEM_BLOCK_START_SIZE;
  }

  block = mem_heap_create_block(NULL, size, filename, line);

  if (block == NULL) {
    return (NULL);
  }

  LIST_INIT(block->base);

  /* Add the created block itself as the first block in the list */
  LIST_ADD_FIRST(list, block->base, block);

  return (block);
}

/** Adds a new block to a memory heap.
 @param[in]     heap    memory heap
 @param[in]     n       number of bytes needed
 @return created block, NULL if did not succeed */
static inline mem_block_t *mem_heap_add_block(mem_heap_t *heap, size_t n)
{
  mem_block_t *block;
  mem_block_t *new_block;
  size_t new_size;

  wl_d(mem_block_validate(heap));

  block = LIST_GET_LAST(heap->base);

  /* We have to allocate a new block. The size is always at least
  doubled until the standard size is reached. After that the size
  stays the same, except in cases where the caller needs more space. */

  new_size = 2 * block->len;

  if (new_size > MEM_BLOCK_STANDARD_SIZE) {
    new_size = MEM_BLOCK_STANDARD_SIZE;
  }

  if (new_size < n) {
    new_size = n;
  }

  new_block = mem_heap_create_block(heap, new_size, heap->file_name, heap->line);
  if (new_block == NULL) {
    return (NULL);
  }

  /* Add the new block as the last block */

  LIST_INSERT_AFTER(list, heap->base, block, new_block);

  return (new_block);
}

/** Allocates n bytes of memory from a memory heap.
@param[in]      heap    memory heap
@param[in]      n       number of bytes; if the heap is allowed to grow into
the buffer pool, this must be <= MEM_MAX_ALLOC_IN_BUF
@return allocated storage, NULL if did not succeed */
static inline void *mem_heap_alloc(mem_heap_t *heap, size_t n) {
  mem_block_t *block;
  byte *buf;
  size_t free;

  wl_d(mem_block_validate(heap));

  block = LIST_GET_LAST(heap->base);

  /* Check if there is enough space in block. If not, create a new
  block to the heap */

  if (block->len < block->free + MEM_SPACE_NEEDED(n)) {
    block = mem_heap_add_block(heap, n);

    if (block == NULL) {
      return (NULL);
    }
  }

  free = block->free;

  buf = (byte *)block + free + MEM_NO_MANS_LAND;

  wl_ad(free + MEM_SPACE_NEEDED(n) >= block->start && free + MEM_SPACE_NEEDED(n) <= block->len);
  block->free = free + MEM_SPACE_NEEDED(n);

  wl_ad(block->len >= block->free);

#ifdef WLIB_DEBUG
  byte *start_no_mans_land = buf - MEM_NO_MANS_LAND;
  byte *end_no_mans_land =
      start_no_mans_land + MEM_SPACE_NEEDED(n) - MEM_NO_MANS_LAND;
  memset(start_no_mans_land, MEM_NO_MANS_LAND_BEFORE_BYTE, MEM_NO_MANS_LAND);

  memset(end_no_mans_land, MEM_NO_MANS_LAND_AFTER_BYTE, MEM_NO_MANS_LAND);
#endif

  return (buf);
}

/** Allocates and zero-fills n bytes of memory from a memory heap.
@param[in]      heap    memory heap
@param[in]      n       number of bytes; if the heap is allowed to grow into
the buffer pool, this must be <= MEM_MAX_ALLOC_IN_BUF
@return allocated, zero-filled storage */
static inline void *mem_heap_zalloc(mem_heap_t *heap, size_t n) {
  wl_ad(heap);
  return (memset(mem_heap_alloc(heap, n), 0, n));
}

#ifdef WLIB_DEBUG
static inline void validate_no_mans_land(byte *no_mans_land_begin, byte mem_no_mans_land_byte) {
  byte *no_mans_land_it;
  for (no_mans_land_it = no_mans_land_begin;
       no_mans_land_it < no_mans_land_begin + MEM_NO_MANS_LAND;
       ++no_mans_land_it) {
    wl_a(*no_mans_land_it == mem_no_mans_land_byte);
  }
}
#endif

/** Frees a block from a memory heap. */
static inline void mem_heap_block_free(mem_heap_t *heap,   /*!< in: heap */
                         mem_block_t *block) /*!< in: block to free */
{
  wl_d(mem_block_validate(block));

  LIST_REMOVE(list, heap->base, block);

  wl_ad(heap->total_size >= block->len);
  heap->total_size -= block->len;

  block->magic_n = MEM_FREED_BLOCK_MAGIC_N;

#ifdef WLIB_DEBUG
  if (block->start != block->free) {
    validate_no_mans_land((byte *)block + block->start, MEM_NO_MANS_LAND_BEFORE_BYTE);
    validate_no_mans_land((byte *)block + block->free - MEM_NO_MANS_LAND, MEM_NO_MANS_LAND_AFTER_BYTE);
  }
#endif

  free(block);
}

/** Frees the space occupied by a memory heap.
@param[in]      heap    Heap to be freed */
static inline void mem_heap_free(mem_heap_t *heap) {
  mem_block_t *block;
  mem_block_t *prev_block;

  wl_d(mem_block_validate(heap));

  block = LIST_GET_LAST(heap->base);

  while (block != NULL) {
    /* Store the contents of info before freeing current block
    (it is erased in freeing) */

    prev_block = LIST_GET_PREV(list, block);

    mem_heap_block_free(heap, block);

    block = prev_block;
  }
}

/** Returns the space in bytes occupied by a memory heap. */
static inline size_t mem_heap_get_size(mem_heap_t *heap) /*!< in: heap */
{
  size_t size = 0;

  wl_d(mem_block_validate(heap));

  size = heap->total_size;

  return (size);
}

/** Returns a pointer to the topmost element in a memory heap.
The size of the element must be given.
@param[in]      heap    memory heap
@param[in]      n       size of the topmost element
@return pointer to the topmost element */
static inline void *mem_heap_get_top(mem_heap_t *heap, size_t n) {
  mem_block_t *block;
  byte *buf;

  wl_d(mem_block_validate(heap));

  block = LIST_GET_LAST(heap->base);

  buf = (byte *)block + block->free - MEM_SPACE_NEEDED(n) + MEM_NO_MANS_LAND;

  return ((void *)buf);
}

/** Returns a pointer to the heap top.
@param[in]      heap    memory heap
@return pointer to the heap top */
static inline byte *mem_heap_get_heap_top(mem_heap_t *heap) {
  mem_block_t *block;
  byte *buf;

  wl_d(mem_block_validate(heap));

  block = LIST_GET_LAST(heap->base);

  buf = (byte *)block + block->free;

  return (buf);
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

  wl_d(mem_block_validate(heap));

  first_free_byte = mem_heap_get_heap_top(heap);

  presumed_start_of_buf = first_free_byte - MEM_SPACE_NEEDED(buf_sz) + MEM_NO_MANS_LAND;

  return (presumed_start_of_buf == buf);
}

/** Frees the topmost element in a memory heap.
@param[in]      heap    memory heap
@param[in]      n       size of the topmost element
The size of the element must be given. */
static inline void mem_heap_free_top(mem_heap_t *heap, size_t n)
{
  mem_block_t *block;

  wl_d(mem_block_validate(heap));

  block = LIST_GET_LAST(heap->base);

  /* Subtract the free field of block */
  wl_ad(block->free - MEM_SPACE_NEEDED(n) >= block->start && block->free - MEM_SPACE_NEEDED(n) <= block->len);
  block->free = block->free - MEM_SPACE_NEEDED(n);
#ifdef WLIB_DEBUG
  validate_no_mans_land((byte *)block + block->free, MEM_NO_MANS_LAND_BEFORE_BYTE);
  validate_no_mans_land((byte *)block + block->free + MEM_SPACE_NEEDED(n) - MEM_NO_MANS_LAND, MEM_NO_MANS_LAND_AFTER_BYTE);
#endif

  /* If free == start, we may free the block if it is not the first
  one */

  if ((heap != block) &&
      (block->free == block->start)) {
    mem_heap_block_free(heap, block);
  }
}

/** Frees the space in a memory heap exceeding the pointer given.
The pointer must have been acquired from mem_heap_get_heap_top.
The first memory block of the heap is not freed.
@param[in]      heap            heap from which to free
@param[in]      old_top         pointer to old top of heap */
static inline void mem_heap_free_heap_top(mem_heap_t *heap, byte *old_top) {
  mem_block_t *block;
  mem_block_t *prev_block;

  wl_d(mem_heap_validate(heap));

  block = LIST_GET_LAST(heap->base);

  while (block != NULL) {
    if (((byte *)block + block->free >= old_top) &&
        ((byte *)block <= old_top)) {
      /* Found the right block */

      break;
    }

    /* Store prev_block value before freeing the current block
    (the current block will be erased in freeing) */

    prev_block = LIST_GET_PREV(list, block);

    mem_heap_block_free(heap, block);

    block = prev_block;
  }

  wl_ad(block);

  if (old_top == (byte *)block + block->free) {
    return;
  }

#ifdef WLIB_DEBUG
  validate_no_mans_land(old_top, MEM_NO_MANS_LAND_BEFORE_BYTE);
  validate_no_mans_land(
      (byte *)block + block->free - MEM_NO_MANS_LAND,
      MEM_NO_MANS_LAND_AFTER_BYTE);
#endif
  /* Set the free field of block */
  wl_ad(old_top - (byte *)block >= block->start && old_top - (byte *)block <= block->len);
  block->free = old_top - (byte *)block;

  wl_ad(block->start <= block->free);

  /* If free == start, we may free the block if it is not the first
  one */

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

/** Allocate a new chunk of memory from a memory heap, possibly discarding the
topmost element. If the memory chunk specified with (top, top_sz) is the
topmost element, then it will be discarded, otherwise it will be left untouched
and this function will be equivallent to mem_heap_alloc().
@param[in,out]  heap    memory heap
@param[in]      top     chunk to discard if possible
@param[in]      top_sz  size of top in bytes
@param[in]      new_sz  desired size of the new chunk
@return allocated storage, NULL if did not succeed */
static inline void *mem_heap_replace(mem_heap_t *heap, const void *top,
                                     size_t top_sz, size_t new_sz)
{
  if (mem_heap_is_top(heap, top, top_sz)) {
    mem_heap_free_top(heap, top_sz);
  }

  return (mem_heap_alloc(heap, new_sz));
}

/** Allocate a new chunk of memory from a memory heap, possibly discarding the
topmost element and then copy the specified data to it. If the memory chunk
specified with (top, top_sz) is the topmost element, then it will be discarded,
otherwise it will be left untouched and this function will be equivalent to
mem_heap_dup().
@param[in,out]  heap    memory heap
@param[in]      top     chunk to discard if possible
@param[in]      top_sz  size of top in bytes
@param[in]      data    new data to duplicate
@param[in]      data_sz size of data in bytes
@return allocated storage, NULL if did not succeed */
static inline void *mem_heap_dup_replace(mem_heap_t *heap, const void *top,
                                         size_t top_sz, const void *data,
                                         size_t data_sz)
{
  void *p = mem_heap_replace(heap, top, top_sz, data_sz);

  memcpy(p, data, data_sz);

  return (p);
}

/** Allocate a new chunk of memory from a memory heap, possibly discarding the
topmost element and then copy the specified string to it. If the memory chunk
specified with (top, top_sz) is the topmost element, then it will be discarded,
otherwise it will be left untouched and this function will be equivalent to
mem_heap_strdup().
@param[in,out]  heap    memory heap
@param[in]      top     chunk to discard if possible
@param[in]      top_sz  size of top in bytes
@param[in]      str     new data to duplicate
@return allocated string, NULL if did not succeed */
static inline char *mem_heap_strdup_replace(mem_heap_t *heap, const void *top,
                                            size_t top_sz, const char *str)
{
  return ((char *)(mem_heap_dup_replace(heap, top, top_sz, str, strlen(str) + 1)));
}

/** Duplicate a block of data, allocated from a memory heap.
 @return own: a copy of the data */
void *mem_heap_dup(
    mem_heap_t *heap, /*!< in: memory heap where copy is allocated */
    const void *data, /*!< in: data to be copied */
    size_t len)        /*!< in: length of data, in bytes */
{
  return (memcpy(mem_heap_alloc(heap, len), data, len));
}

/** Duplicates a NUL-terminated string, allocated from a memory heap.
@param[in]      heap    memory heap where string is allocated
@param[in]      str     string to be copied
@return own: a copy of the string */
char *mem_heap_strdup(mem_heap_t *heap, const char *str) {
  return ((char *)(mem_heap_dup(heap, str, strlen(str) + 1)));
}

/** Makes a NUL-terminated copy of a nonterminated string, allocated from a
memory heap.
@param[in]      heap    memory heap where string is allocated
@param[in]      str     string to be copied
@param[in]      len     length of str, in bytes
@return own: a copy of the string */
static inline char *mem_heap_strdupl(mem_heap_t *heap, const char *str, size_t len)
{
  char *s = (char *)mem_heap_alloc(heap, len + 1);
  s[len] = 0;
  if (len > 0) {
    memcpy(s, str, len);
  }
  return s;
}

/** Concatenate two strings and return the result, using a memory heap.
 @return own: the result */
char *mem_heap_strcat(
    mem_heap_t *heap, /*!< in: memory heap where string is allocated */
    const char *s1,   /*!< in: string 1 */
    const char *s2)   /*!< in: string 2 */
{
  char *s;
  size_t s1_len = strlen(s1);
  size_t s2_len = strlen(s2);

  s = (char *)(mem_heap_alloc(heap, s1_len + s2_len + 1));

  memcpy(s, s1, s1_len);
  memcpy(s + s1_len, s2, s2_len);

  s[s1_len + s2_len] = '\0';

  return (s);
}

/** Duplicates a NUL-terminated string.
@param[in]      str     string to be copied
@return own: a copy of the string, must be deallocated with free */
static inline char *mem_strdup(const char *str)
{
  size_t len = strlen(str) + 1;
  return ((char *)(
      memcpy(malloc(len), str, len)));
}

/** Makes a NUL-terminated copy of a nonterminated string.
@param[in]      str     string to be copied
@param[in]      len     length of str, in bytes
@return own: a copy of the string, must be deallocated with free */
static inline char *mem_strdupl(const char *str, size_t len)
{
  char *s = (char *)(
      malloc(len + 1));
  s[len] = 0;
  if (len > 0) {
    memcpy(s, str, len);
  }
  return s;
}

/** Helper function for mem_heap_printf.
 @return length of formatted string, including terminating NUL */
static size_t mem_heap_printf_low(
    char *buf,          /*!< in/out: buffer to store formatted string
                        in, or NULL to just calculate length */
    const char *format, /*!< in: format string */
    va_list ap)         /*!< in: arguments */
{
  size_t len = 0;

  while (*format) {
    /* Does this format specifier have the 'l' length modifier. */
    bool is_long = false;

    /* Length of one parameter. */
    size_t plen;

    if (*format++ != '%') {
      /* Non-format character. */

      len++;

      if (buf) {
        *buf++ = *(format - 1);
      }

      continue;
    }

    if (*format == 'l') {
      is_long = true;
      format++;
    }

    switch (*format++) {
      case 's':
        /* string */
        {
          char *s = va_arg(ap, char *);

          /* "%ls" is a non-sensical format specifier. */
          wl_a(!is_long);

          plen = strlen(s);
          len += plen;

          if (buf) {
            memcpy(buf, s, plen);
            buf += plen;
          }
        }

        break;

      case 'u':
        /* unsigned int */
        {
          char tmp[32];
          unsigned long val;

          /* We only support 'long' values for now. */
          wl_a(is_long);

          val = va_arg(ap, unsigned long);

          plen = sprintf(tmp, "%lu", val);
          len += plen;

          if (buf) {
            memcpy(buf, tmp, plen);
            buf += plen;
          }
        }

        break;

      case '%':

        /* "%l%" is a non-sensical format specifier. */
        wl_a(!is_long);

        len++;

        if (buf) {
          *buf++ = '%';
        }

        break;

      default:
        wl_error;
    }
  }

  /* For the NUL character. */
  len++;

  if (buf) {
    *buf = '\0';
  }

  return (len);
}

/** A simple sprintf replacement that dynamically allocates the space for the
 formatted string from the given heap. This supports a very limited set of
 the printf syntax: types 's' and 'u' and length modifier 'l' (which is
 required for the 'u' type).
 @return heap-allocated formatted string */
char *mem_heap_printf(mem_heap_t *heap,   /*!< in: memory heap */
                      const char *format, /*!< in: format string */
                      ...) {
  va_list ap;
  char *str;
  size_t len;

  /* Calculate length of string */
  len = 0;
  va_start(ap, format);
  len = mem_heap_printf_low(NULL, format, ap);
  va_end(ap);

  /* Now create it for real. */
  str = (char *)(mem_heap_alloc(heap, len));
  va_start(ap, format);
  mem_heap_printf_low(str, format, ap);
  va_end(ap);

  return (str);
}

#endif