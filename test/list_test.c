#include "list.h"
int main(int argc, char const *argv[])
{
    list_t *list = list_create(NULL);
    list_add_last(list, (void *)123);
    list_free(list);
    return 0;
}
