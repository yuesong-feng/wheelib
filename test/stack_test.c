#include "stack.h"
#include <stdio.h>
#include <string.h>

struct node {
    int id;
    char name[32];
};

int main(int argc, char const *argv[])
{
    stack_t stk;
    stack_init(&stk, sizeof(struct node));

    struct node n;
    for (int i = 0; i < 10; ++i) {
        n.id = i;
        sprintf(n.name, "str:%d", i);
        stack_push(&stk, &n);
    }

    for (int i = 0; i < 10; ++i) {
        struct node *get = stack_top(&stk);
        printf("%d, %s\n", get->id, get->name);
        stack_pop(&stk);
    }

    stack_destroy(&stk);
    return 0;
}
