/*

  basic.h

  basic utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05
*/
#ifndef BASIC_H
#define BASIC_H
#include <stdlib.h> // for exit(EXIT_FAILURE)

#ifdef NDEBUG
  #undef UNIV_DEBUG
#elif !defined WLIB_DEBUG
  #define WLIB_DEBUG
#endif

// <stdint.h> will be used to determine 32/64 bit platform
#if __WORDSIZE == 64
  #define WLIB64
#elif __WORDSIZE == 32
  #define WLIB32
#else
  #ifdef _WIN32
    #ifdef _WIN64
      #define WLIB64
    #else
      #define WLIB32
    #endif
  #else
    #ifdef __x86_64__
      #define WLIB64
    #elif __i386__
      #define WLIB32
    #endif
  #endif
#endif

#define FLEXIBLE_ARRAY_MEMBER /* empty */

#if __GNUC__ >= 3
  #define likely(x) __builtin_expect(!!(x), 1)
  #define unlikely(x) __builtin_expect(!!(x), 0)
#else
  #define likely(x) (x)
  #define unlikely(x) (x)
#endif

/** Abort execution if EXPR does not evaluate to nonzero.
@param EXPR assertion expression that should hold */
#define ast(EXPR)                          \
  do {                                     \
    if (unlikely(false == (bool)(EXPR))) { \
      exit(EXIT_FAILURE);                  \
    }                                      \
  } while (0)

#ifdef WLIB_DEBUG
  /** Debug assertion. Does nothing unless UNIV_DEBUG is defined. */
  #define astd(EXPR) ast(EXPR)
  /** Debug statement. Does nothing unless UNIV_DEBUG is defined. */
  #define ifdbg(EXPR) EXPR
#else
  /** Debug assertion. Does nothing unless UNIV_DEBUG is defined. */
  #define astd(EXPR)
  /** Debug statement. Does nothing unless UNIV_DEBUG is defined. */
  #define ifdbg(EXPR)
#endif

#ifdef WLIB_DEBUG
  #define IF_DEBUG(...) __VA_ARGS__
#else
  #define IF_DEBUG(...)
#endif

#endif