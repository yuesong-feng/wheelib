/*

  dbg.h

  dbg utilities, from MySQL 8.4.0 InnoBase

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 2.0 2024/10/29 create
*/
#ifndef DBG_H
#define DBG_H

#include <stdlib.h>
#include "basic.h"

#define WLIB_DEBUG

/** Assert that LHS OP RHS, where OP is an operator.
Abort execution otherwise.
Technical remarks: The LHS and RHS are evaluated exactly once (no short
circuiting, even if OP is && or ||). Each value is stored in a local variable,
so it's fine for LHS or RHS to return a temporary.
In case of assertion failure references to const values of LHS and RHS will be
passed to std::ostringstream::operator<<, so it must be implemented for them. */
#define wl_a_op(LHS, OP, RHS)                                                  \
  do {                                                                         \
    const auto lhs{LHS};                                                       \
    const auto rhs{RHS};                                                       \
    if (unlikely(!(lhs OP rhs))) {                                             \
      wl_error; \
    }                                                                          \
  } while (0)

/** Assert that LHS < RHS. Abort execution otherwise. */
#define wl_a_lt(LHS, RHS) wl_a_op(LHS, <, RHS)
/** Assert that LHS <= RHS. Abort execution otherwise. */
#define wl_a_le(LHS, RHS) wl_a_op(LHS, <=, RHS)
/** Assert that LHS == RHS. Abort execution otherwise. */
#define wl_a_eq(LHS, RHS) wl_a_op(LHS, ==, RHS)
/** Assert that LHS != RHS. Abort execution otherwise. */
#define wl_a_ne(LHS, RHS) wl_a_op(LHS, !=, RHS)

/** Abort execution if EXPR does not evaluate to nonzero.
@param EXPR assertion expression that should hold */
#define wl_a(EXPR)                                        \
  do {                                                    \
    if (unlikely(false == (bool)(EXPR))) {                \
      wl_error; \
    }                                                     \
  } while (0)

/** Abort execution. */
#define wl_error exit(EXIT_FAILURE)

#ifdef WLIB_DEBUG
/** Debug assertion. Does nothing unless WLIB_DEBUG is defined. */
#define wl_ad(EXPR) wl_a(EXPR)
/** Debug statement. Does nothing unless WLIB_DEBUG is defined. */
#define wl_d(EXPR) EXPR
/** Opposite of wl_d().  Does nothing if WLIB_DEBUG is defined. */
#define wl_o(EXPR)
/** Debug-only assertion that LHS < RHS. */
#define wl_ad_lt(LHS, RHS) wl_a_lt(LHS, RHS)
/** Debug-only assertion that LHS <= RHS. */
#define wl_ad_le(LHS, RHS) wl_a_le(LHS, RHS)
/** Debug-only assertion that LHS == RHS. */
#define wl_ad_eq(LHS, RHS) wl_a_eq(LHS, RHS)
/** Assert that LHS != RHS. Abort execution otherwise. */
#define wl_ad_ne(LHS, RHS) wl_a_op(LHS, !=, RHS)
#else
/** Debug assertion. Does nothing unless WLIB_DEBUG is defined. */
#define wl_ad(EXPR)
/** Debug statement. Does nothing unless WLIB_DEBUG is defined. */
#define wl_d(EXPR)
/** Opposite of wl_d().  Does nothing if WLIB_DEBUG is defined. */
#define wl_o(EXPR) EXPR
/** Debug-only assertion that LHS < RHS. */
#define wl_ad_lt(LHS, RHS)
/** Debug-only assertion that LHS <= RHS. */
#define wl_ad_le(LHS, RHS)
/** Debug-only assertion that LHS == RHS. */
#define wl_ad_eq(LHS, RHS)
/** Assert that LHS != RHS. */
#define wl_ad_ne(LHS, RHS)
#endif

#endif
