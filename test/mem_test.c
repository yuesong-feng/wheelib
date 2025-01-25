#include "mem.h"

int main(int argc, char const *argv[]) {
  mem_t *mem = mem_create(mem_type_os);

  char *str = mem_alloc(mem, 64);
  memset(str, 'a', 64);
  printf("%s\n", str);
  mem_free(mem, str);
  mem_destroy(mem);

  mem = mem_create(mem_type_heap);
  str = mem_alloc(mem, 64);
  memset(str, 'a', 64);
  printf("%s\n", str);
  mem_free(mem, str);
  mem_destroy(mem);

  return 0;
}
