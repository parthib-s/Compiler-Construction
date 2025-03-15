#ifndef STACKADTDEF
#define STACKADTDEF
#include "parserDef.h"
#include "lexerDef.h"

typedef struct {
    ParseTreeNode **nodes;
    int top;
    int capacity;
} Stack;

#endif // !STACKADTDEF
