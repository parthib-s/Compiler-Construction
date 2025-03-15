/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 
 #include "lexerDef.h"
 
 struct element{
     TokenName type;
     char* lexeme;
     struct element* next;
 };
 
 typedef struct element parserElement;
 
 struct parserlist{
     parserElement* head;
 };
 
 typedef struct parserlist parserList;
 
 struct parser{
     int num_pos;
     parserList** positions;
 };	
 
 typedef struct parser parserTable;
 
 //Function Declarations
 parserTable* createParserHashTable(int num_pos);
 void insertElementPHT(parserTable* lookUpTable, char* key, TokenName tokentype);
 int hashFuncPHT(char* key, int num_pos);
 void fillPHT(parserTable* lookUpTable);
 TokenName getTokenTypePHT(char* key, parserTable* lookUpTable);
 
 