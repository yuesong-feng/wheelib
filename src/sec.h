/**
 * @file sec.h
 * @date 2025-01-25
 * @author yuesong-feng
 */
#ifndef SEC_H
#define SEC_H
#include <sys/time.h>

typedef unsigned long int sec_t;
typedef unsigned long int msec_t;
typedef unsigned long int usec_t;
typedef unsigned long int nsec_t;

#define MSEC_PER_SEC 1000
#define USEC_PER_SEC 1000000
#define NSEC_PER_SEC 1000000000

#define USEC_PER_MSEC 1000
#define NSEC_PER_MSEC 1000000

#define NSEC_PER_USEC 1000

void timeval_now(struct timeval *tv);

void timeval_from_usec(struct timeval *tv, usec_t usec);

void timeval_add_usec(struct timeval *tv, usec_t usec);

void timespec_from_timeval(struct timespec *ts, const struct timeval *tv);

#endif