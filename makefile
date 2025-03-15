parser: parser.o lexer.o driver.o keyword_table.o 
	gcc parser.o lexer.o driver.o keyword_table.o -o myfinalprogram

lexer: lexer.o keyword_table.o driver.o
	gcc lexer.o keyword_table.o driver.o -o myprogram

lexer.o: lexer.c lexer.h lexerDef.h
	gcc -w -c lexer.c

keyword_table.o: keyword_table.c keyword_table.h keyword_tableDef.h
	 gcc -w -c keyword_table.c

parser.o: parser.c parser.h parserDef.h lexer.h
	gcc -w -c parser.c

driver.o: driver.c
	 gcc -w -c driver.c