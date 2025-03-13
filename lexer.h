#ifndef LEXER
#define LEXER
#include "lexerDef.h"
#include <stdio.h>
FILE* getStream(FILE *fp,twinBuffer* TB);

void retract(int amt,twinBuffer* B);

int checkSingleChar(char ch, char chToEqual);

void accept(twinBuffer* B);

void initBuffer(int size);

int getNextChar(twinBuffer* B);

void initLexer(FILE* f);



char* copyString(int start, int end);

tokenInfo createToken(TokenName tokenName,char* lexeme,int lineNumber,int isNumber,Value* value);

int checkInRange(char ch,char start, char end);
tokenInfo getNextToken();
void removeComments(char *testcaseFile, char *cleanFile);

#endif


