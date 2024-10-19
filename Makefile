all:
	bison -d par.y
	flex lex.l
	gcc lex.yy.c par.tab.c