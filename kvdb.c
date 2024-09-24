#include "hash.h"
#include "io.h"

typedef struct item item;
struct item {
  char key[1024];
  char val[1024];
  hash_node_t link;
};

int main(int argc, char const *argv[]) {
  hash_table_t *db = hash_create(1024);
  mem_heap_t *heap = mem_heap_create(0);
  char op[10];
  char key[1024];
  char val[1024];
  item *it;

  while (true) {
    get_word(stdin, op);
    if (strcmp(op, "set") == 0) {
      get_word(stdin, key);
      get_word(stdin, val);
      it = mem_heap_zalloc(heap, sizeof(item));
      strcpy(it->key, key);
      strcpy(it->val, val);
      HASH_INSERT(item, link, db, atoi(it->key), it);
    } else if (strcmp(op, "get") == 0) {
      get_word(stdin, key);
      HASH_SEARCH(link, db, atoi(key), item *, it, strcmp(key, it->key) == 0);
      if (it == NULL) {
        printf("NULL\n");
      } else {
        printf("find node, val: %s\n", it->val);
      }
    } else if (strcmp(op, "show") == 0) {
      int i;
      for (i = 0; i < db->n_cells; ++i) {
        hash_cell_t *cell = hash_get_nth_cell(db, i);
        while (cell != NULL) {
          it = cell->node;
          printf("node, key: %s, val: %s\n", it->key, it->val);
          cell = HASH_GET_NEXT(link, it);
        }
      }
    }
  }

  mem_heap_free(heap);
  hash_table_free(db);
  return 0;
}
