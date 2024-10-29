all:
	bison -d par.y
	flex lex.l
	gcc lex.yy.c par.tab.c

.PHONY: mem_test hash_test

mem_test:
	gcc mem_test.c -o mem_test

hash_test:
	gcc hash_test.c -o hash_test