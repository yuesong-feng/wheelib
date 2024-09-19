# wheelib

Design Philosophy of wheelib:

simple(简单) + tidy(整洁) = beautiful art

wlib is header-only, avoiding many problems related to compiling, static/dynamic library, etc.

wlib trust operation system by considering all system calls should return properly, which may be dangerous in production environment requiring high stability.

wlib is for unix system now, Windows may be supported later.

wlib includes C99 <stdbool.h> for delicate bool/true/false

wlib includes C99 <stdint.h> because it's really hard for the lib to perfectly define 32/64 bit platform.

wlib doesn't include <stdio.h>

wlib dependency graph:

    rwlock/thread
        basic
    event/sem
        calc
        cond
            mutex
                basic
    hash/stack
        mem
            lst
            byte
                basic
            calc
                basic

git push -u origin main

stable release(won't change until next version):
    lst hash mem mutex cond rwlock event sem basic byte calc