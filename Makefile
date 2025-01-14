

wheelib:
	gcc -shared -fPIC -o libwheelib.so src/mem.c src/list.c src/alloc.c src/vec.c

.PHONY: mem_test list_test vec_test all

mem_test: wheelib
	gcc test/mem_test.c -Isrc -L./ -lwheelib -o mem_test

list_test: wheelib
	gcc test/list_test.c -Isrc -L./ -lwheelib -o list_test

vec_test: wheelib
	gcc test/vec_test.c -Isrc -L./ -lwheelib -o vec_test

all: mem_test list_test vec_test