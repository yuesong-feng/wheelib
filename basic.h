/*

  basic.h

  basic utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 1.0 2024/09/05
*/
#ifndef BASIC_H
#define BASIC_H
#include <stdlib.h> // for exit(EXIT_FAILURE)
#include <sys/time.h> // for gettimeofday

// <stdint.h> will be used to determine 32/64 bit platform
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

#define FLEXIBLE_ARRAY_MEMBER /* empty */

#if __GNUC__ >= 3
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

#define halt_if(cond) if (unlikely(cond)) exit(EXIT_FAILURE);

#endif