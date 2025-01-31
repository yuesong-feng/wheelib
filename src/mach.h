/**
 * @file mach.h
 * @date 2025-01-29
 * @author yuesong-feng
 */
#ifndef MACH_H
#define MACH_H
#include "byte.h"
#include <stddef.h>
#include <stdint.h>

void mach_write_to_1(byte *b, size_t n);

size_t mach_read_from_1(const byte *b);

void mach_write_to_2(byte *b, size_t n);

size_t mach_read_from_2(const byte *b);

void mach_write_to_3(byte *b, size_t n);

size_t mach_read_from_3(const byte *b);

void mach_write_to_4(byte *b, size_t n);

size_t mach_read_from_4(const byte *b);

void mach_write_to_6(void *b, uint64_t n);

uint64_t mach_read_from_6(const byte *b);

void mach_write_to_7(void *b, uint64_t n);

uint64_t mach_read_from_7(const byte *b);

void mach_write_to_8(void *b, uint64_t n);

uint64_t mach_read_from_8(const byte *b);

#endif