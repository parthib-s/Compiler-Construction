#include "parser.h"
#include "lexer.h"
#include "stdio.h"
#define MAX_LINE_LENGTH 1000
#define MAX_TOKENS_PER_PRODUCTION 10
Grammar* g;

int loadGrammar(const char *filename, Grammar *grammar) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return -1;
    }
    
    // Allocate an initial array for productions
    int capacity = 20;
    grammar->productions = (Production *)malloc(capacity * sizeof(Production));
    if (grammar->productions == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(fp);
        return -1;
    }
    grammar->productionCount = 0;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Remove any trailing newline characters.
        line[strcspn(line, "\n")] = '\0';

        // Skip empty lines
        if (strlen(line) == 0)
            continue;

        // Expand productions array if needed.
        if (grammar->productionCount == capacity) {
            capacity *= 2;
            Production *temp = (Production *)realloc(grammar->productions, capacity * sizeof(Production));
            if (temp == NULL) {
                fprintf(stderr, "Error: Memory reallocation failed\n");
                fclose(fp);
                return -1;
            }
            grammar->productions = temp;
        }
        
        // Tokenize the line. First token is the LHS.
        char *token = strtok(line, " \t");
        if (token == NULL)
            continue; // Skip empty or malformed lines
        
        // Initialize the production rule.
        Production *prod = &grammar->productions[grammar->productionCount];
        strncpy(prod->lhs, token, MAX_TOKEN_LENGTH - 1);
        prod->lhs[MAX_TOKEN_LENGTH - 1] = '\0';
        
        // Allocate memory for RHS tokens.
        prod->rhs = (char **)malloc(MAX_TOKENS_PER_PRODUCTION * sizeof(char *));
        prod->rhsCount = 0;
        if (prod->rhs == NULL) {
            fprintf(stderr, "Error: Memory allocation for RHS failed\n");
            fclose(fp);
            return -1;
        }
        
        // Parse remaining tokens as RHS symbols.
        while ((token = strtok(NULL, " \t")) != NULL) {
            // Allocate and copy each token.
            prod->rhs[prod->rhsCount] = (char *)malloc((strlen(token) + 1) * sizeof(char));
            if (prod->rhs[prod->rhsCount] == NULL) {
                fprintf(stderr, "Error: Memory allocation for token failed\n");
                fclose(fp);
                return -1;
            }
            strcpy(prod->rhs[prod->rhsCount], token);
            prod->rhsCount++;
            
            // Check maximum tokens limit.
            if (prod->rhsCount >= MAX_TOKENS_PER_PRODUCTION) {
                break;
            }
        }
        
        grammar->productionCount++;
    }
    
    fclose(fp);
    return 0;
}

void printGrammar(const Grammar *grammar) {
    for (int i = 0; i < grammar->productionCount; i++) {
        const Production *prod = &grammar->productions[i];
        printf("%s -> ", prod->lhs);
        for (int j = 0; j < prod->rhsCount; j++) {
            printf("%s ", prod->rhs[j]);
        }
        printf("\n");
    }
}

// FirstAndFollow ComputeFirstAndFollowSets(grammar G) {
//     FirstAndFollow F;
//     // Compute or load FIRST and FOLLOW sets here
//     return F;
// }

// ParseTable createParseTable(FirstAndFollow F, grammar G) {
//     ParseTable T;
//     // Allocate and fill the parse table using FIRST and FOLLOW sets
//     return T;
// }

// ParseTree parseInputSourceCode(char *testcaseFile, ParseTable T) {
//     // Open file and initialize twinBuffer, etc.
//     FILE *fp = fopen(testcaseFile, "r");
//     if (!fp) {
//         fprintf(stderr, "Error opening file %s\n", testcaseFile);
//         exit(EXIT_FAILURE);
//     }
    
    
//     // Initialize parse tree root with start symbol of grammar
//     ParseTree root = malloc(sizeof(parseTreeNode));
//     // Set root details, e.g., strcpy(root->nonTerminal, "START");
    
//     // Initialize parsing stack and push start symbol
//     // while token != EOF, apply predictive parsing using T and update parse tree
//     tokenInfo token;
//     while ((token = getNextToken()).tokenType != EOF_TOKEN) {
//         // Find matching production in parse table
//         // If match, create new parse tree node, add as child of current node
//         // Else, report error with token.lineNo and token.lexeme
//     }
    
//     fclose(fp);
//     printf("Input source code is syntactically correct...........\n");
//     return root;
// }

// void printParseTree(ParseTree PT, char *outfile) {
//     FILE *fp = fopen(outfile, "w");
//     if (!fp) {
//         fprintf(stderr, "Error opening output file %s\n", outfile);
//         return;
//     }
//     // Inorder traversal of parse tree printing details as:
//     // lexeme, current node, lineNo, tokenName, value if number, parent node symbol, isLeaf(yes/no), node symbol
//     // For non-leaf nodes, print dummy lexeme "----"
//     // Implement traversal recursively
//     fclose(fp);
// }

int main(){
    loadGrammar("grammar.txt",g);
    //printGrammar(g);
    return 0;
}
