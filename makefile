parser: parser.o lexer.o driver.o keyword_table.o stackADT.o setADT.o hashtable.o
	gcc parser.o lexer.o driver.o keyword_table.o stackADT.o setADT.o hashtable.o -o myfinalprogram

lexer: lexer.o keyword_table.o driver.o
	gcc lexer.o keyword_table.o driver.o -o myprogram

lexer.o: lexer.c lexer.h lexerDef.h
	gcc -w -c lexer.c

keyword_table.o: keyword_table.c keyword_table.h keyword_tableDef.h
	 gcc -w -c keyword_table.c

parser.o: parser.c parser.h parserDef.h interface.h lexer.h
	gcc -w -c parser.c

stackADT.o : stackADT.c stackADT.h stackADTDef.h
	gcc -w -c stackADT.c

setADT.o : setADT.c setADT.h stackADTDef.h
	gcc -w -c setADT.c

hashtable.o : hashtable.c hashtable.h hashtableDef.h
	gcc -w -c hashtable.c

driver.o: driver.c
	 gcc -w -c driver.c