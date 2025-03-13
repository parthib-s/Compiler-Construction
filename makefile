parser: parser.o lexer.o driver.o keyword_table.o nary_tree.o stack.o
	gcc parser.o lexer.o driver.o keyword_table.o nary_tree.o stack.o -o myfinalprogram

lexer: lexer.o keyword_table.o driver.o
	gcc lexer.o keyword_table.o driver.o -o myprogram

lexer.o: lexer.c lexer.h lexerDef.h
	gcc -w -c lexer.c

keyword_table.o: keyword_table.c keyword_table.h keyword_tableDef.h
	 gcc -w -c keyword_table.c

parser.o: parser.c parser.h parserDef.h interface.h lexer.h
	gcc -w -c parser.c

nary_tree.o : nary_tree.c interface.h nary_treeDef.h nary_tree.h
	gcc -w -c nary_tree.c

stack.o: stack.c stackDef.h nary_tree.h interface.h
	gcc -w -c stack.c

driver.o: driver.c
	 gcc -w -c driver.c