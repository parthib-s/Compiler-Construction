#ifndef STACKADT
#define STACKADT
#include "stackADTDef.h"
#include "parserDef.h"
#include "lexerDef.h"

Stack* createStack(int capacity);
void push(Stack* s, ParseTreeNode* node);
ParseTreeNode* pop(Stack* s);
ParseTreeNode* peek(Stack* s);

#endif // !STACKADT