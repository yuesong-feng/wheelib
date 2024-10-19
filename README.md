# wheelib

wheelib(Whee[wiː] Lib) is a industrial-grade C library.

Design Philosophy of wheelib: simple(简单) + tidy(整洁)

wlib is header-only, avoiding many problems related to compiling, static/dynamic library, etc.

wlib trust operation system by considering all system calls should return properly, which may be dangerous in production environment requiring high stability.

wlib is for unix system now, Windows may be supported later.

wlib includes C99 <stdbool.h> for delicate bool/true/false

wlib includes C99 <stdint.h> because it's really hard for the lib itself to perfectly define 32/64 bit platform.

wlib doesn't include <stdio.h> now

|  modular   | description  |
|  ----      | ----  |
| basic      | some basic defines |
| byte       | byte utilities |
| calc       | calculations |
| mutex      | mutex lock |
| rwlock     | read-write lock |
| cond       | conditional variable |
| event      | event utilities |
| sem        | semaphore utilities |
| mem        | memory utilities |
| thread     | thread utilities |
| lst        | double-linked list |
| hash       | simple static hash table |
| atomic     | atomic utilities |
| io         | input-output utilities |
| vm         | stack based cpu virtual machine |

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
