all:
	bison -d par.y
	flex lex.l
	gcc lex.yy.c par.tab.c

.PHONY: mem_test

mem_test:
	gcc mem_test.c -o mem_test