#ifndef PARSER
#define PARSER
#include "parserDef.h"

int getNonterminalIndex(const char* nt);
int getTerminalIndex(const char* t);
void readGrammar(char* filename);
void computeFirstSets();
void printFirstSets();
void computeFollowSets();
void printFollowSets();
void computeParseTable();
void printParseTable();
ParseTreeNode* createNode(const char* symbol);
Stack* createStack(int capacity);
void push(Stack* s, ParseTreeNode* node);
ParseTreeNode* pop(Stack* s);
ParseTreeNode* peek(Stack* s);
ParseTreeNode* parseInputSourceCode();
void initParser(char * filename);
#endif // !PARSER