all: heap_test list_test vec_test

wheelib:
	gcc -shared -fPIC -o libwheelib.so $(wildcard src/*.c)

.PHONY: heap_test list_test vec_test all

heap_test: wheelib
	gcc test/heap_test.c -Isrc -L./ -lwheelib -o heap_test

list_test: wheelib
	gcc test/list_test.c -Isrc -L./ -lwheelib -o list_test

vec_test: wheelib
	gcc test/vec_test.c -Isrc -L./ -lwheelib -o vec_test

mem_test: wheelib
	gcc test/mem_test.c -Isrc -L./ -lwheelib -o mem_test
