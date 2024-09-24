/*

  io.h

  input-output utilities

  Copyright (c) 2024 yuesong-feng 冯岳松

  Version 0.1 2024/09/20 created

*/
#ifndef IO_H
#define IO_H
#include "basic.h"
#include <stdio.h>

static inline void get_word(FILE *file, char *out) {
  int ret = fscanf(file, "%s", out);
  if (unlikely(ret == EOF))
    out[0] = '\0';
  halt_if(ret != 1);
}

#endif