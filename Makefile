

wheelib:
	gcc -shared -fPIC -o libwheelib.so src/mem.c src/list.c

.PHONY: mem_test

mem_test: wheelib
	gcc test/mem_test.c -Isrc -L./ -lwheelib -o mem_test

list_test: wheelib
	gcc test/list_test.c -Isrc -L./ -lwheelib -o list_test