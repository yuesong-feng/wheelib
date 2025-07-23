/**
 * @file byte.h
 * @date 2025-01-31
 * @author yuesong-feng
 */
#ifndef BYTE_H
#define BYTE_H
#include <stddef.h>
#include <stdint.h>

typedef unsigned char byte;

void write1b(byte *b, uint8_t n);

uint8_t read1b(const byte *b);

void write2b(byte *b, uint16_t n);

uint16_t read2b(const byte *b);

void write4b(byte *b, uint32_t n);

uint32_t read4b(const byte *b);

void write8b(void *b, uint64_t n);

uint64_t read8b(const byte *b);

#endif