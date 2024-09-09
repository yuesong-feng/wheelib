#include "stack.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    stack_t *stk = stack_create();
    byte buf[10];
    
    while (true)
    {
        fgets(buf, 10, stdin);
        if (buf[0] == '+' && buf[1] == '\n')
        {
            int a = stack_pop_8b(stk);
            int b = stack_pop_8b(stk);
            int c = a + b;
            printf("a + b: %d + %d = %d\n", a, b, c);
            stack_push_8b(stk, c);
        }
        else    // number
        {
            stack_push_8b(stk, atoi(buf));
        }
    }

    stack_free(stk);
    return 0;
}
