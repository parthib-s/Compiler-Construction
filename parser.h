#ifndef PARSER
#define PARSER
#include "parserDef.h"

FirstAndFollow ComputeFirstAndFollowSets(grammar G);
ParseTable createParseTable(FirstAndFollow F, grammar G);
ParseTree parseInputSourceCode(char *testcaseFile, ParseTable T);
void printParseTree(ParseTree PT, char *outfile);

#endif // !PARSER