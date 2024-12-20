/*

  basic.h

  basic utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05
*/
#ifndef BASIC_H
#define BASIC_H
#include <stdlib.h> // for exit(EXIT_FAILURE)

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
  #define likely(x) __builtin_expect((x), 1)
  #define unlikely(x) __builtin_expect((x), 0)
#else
  #define likely(x) (x)
  #define unlikely(x) (x)
#endif

#endif