 #ifndef PARSER
 #define PARSER
 
 #include "parserDef.h"
 #include "lexerDef.h"
 
 int* calculateFirst(int production, Grammar g, FirstSet firstSet);
 int* add(int* ans, int* addit);
 void buildFirstSet(Grammar g, FirstSet firstSet);
 void getGram(char *fname, Grammar g);
 void getFollowSets(Grammar g, FollowSet fw, FirstSet fr);
 void findFollow(int NonTerminalNo, int productionNo, Grammar g, FollowSet fw, FirstSet fr,int* flag);
 void createParseTable(FirstSet firstSet,FollowSet followSet,Grammar g,Table t);
 void add2(int* ans,int* add,int* flag);
 void parseInputSourceCode(FILE* sourceFile,Table t,Grammar g,parseTree root,int* error);
 void printParseTree(parseTree root);
 
 
 #endif