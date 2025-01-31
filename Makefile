all: heap_test vec_test hash_test

wheelib:
	gcc -g -shared -fPIC -o libwheelib.so $(wildcard src/*.c)

.PHONY:  all

heap_test: wheelib
	gcc test/heap_test.c -Isrc -L./ -lwheelib -o heap_test

vec_test: wheelib
	gcc test/vec_test.c -Isrc -L./ -lwheelib -o vec_test

mem_test: wheelib
	gcc test/mem_test.c -Isrc -L./ -lwheelib -o mem_test

event_test: wheelib
	gcc -g test/event_test.c -Isrc -L./ -lwheelib -o event_test

rwlock_test: wheelib
	gcc -g test/rwlock_test.c -Isrc -L./ -lwheelib -o rwlock_test

thread_test: wheelib
	gcc -g test/thread_test.c -Isrc -L./ -lwheelib -o thread_test

sema_test: wheelib
	gcc -g test/sema_test.c -Isrc -L./ -lwheelib -o sema_test

hash_test: wheelib
	gcc -g test/hash_test.c -Isrc -L./ -lwheelib -o hash_test