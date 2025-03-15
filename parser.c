#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_TERMINALS 61
#define NUM_NON_TERMINALS 53
#define MAX_PRODUCTIONS 200

typedef struct {
    char *lhs;
    char **rhs;
    int rhsCount;
} Production;

Production productions[MAX_PRODUCTIONS];
int prodCount = 0;

// Global array of terminals
const char *terminals[NUM_TERMINALS] = {
    "TK_UNKNOWN", "TK_ASSIGNOP", "TK_COMMENT", "TK_ID", "TK_RUID",
    "TK_UNION", "TK_ENDUNION", "TK_DEFINETYPE", "TK_AS", "TK_NUM",
    "TK_RNUM", "TK_FIELDID", "TK_FUNID", "TK_WITH", "TK_PARAMETERS",
    "TK_END", "TK_WHILE", "TK_TYPE", "TK_MAIN", "TK_GLOBAL", "TK_PARAMETER",
    "TK_LIST", "TK_SQL", "TK_SQR", "TK_INPUT", "TK_OUTPUT", "TK_INT",
    "TK_REAL", "TK_COMMA", "TK_SEM", "TK_COLON", "TK_DOT", "TK_ENDWHILE",
    "TK_OP", "TK_CL", "TK_IF", "TK_THEN", "TK_ENDIF", "TK_READ", "TK_WRITE",
    "TK_RETURN", "TK_PLUS", "TK_MINUS", "TK_MUL", "TK_DIV", "TK_CALL",
    "TK_RECORD", "TK_ENDRECORD", "TK_ELSE", "TK_AND", "TK_OR", "TK_NOT",
    "TK_LT", "TK_LE", "TK_EQ", "TK_GT", "TK_GE", "TK_NE", "TK_ERR",
    "TK_EPS", "TK_DOLLAR"
};

// Global array of nonterminals
char *nonterminals[NUM_NON_TERMINALS] = {
    "program",
    "otherFunctions",
    "mainFunction",
    "stmts",
    "function",
    "input_par",
    "output_par",
    "parameter_list",
    "dataType",
    "remaining_list",
    "primitiveDatatype",
    "constructedDatatype",
    "typeDefinitions",
    "declarations",
    "otherStmts",
    "returnStmt",
    "actualOrRedefined",
    "typeDefinition",
    "definetypestmt",
    "fieldDefinitions",
    "fieldDefinition",
    "moreFields",
    "fieldtype",
    "declaration",
    "global_or_not",
    "stmt",
    "assignmentStmt",
    "iterativeStmt",
    "conditionalStmt",
    "ioStmt",
    "funCallStmt",
    "singleOrRecId",
    "arithmeticExpression",
    "option_single_constructed",
    "oneExpansion",
    "moreExpansions",
    "outputParameters",
    "inputParameters",
    "idList",
    "booleanExpression",
    "elsePart",
    "var",
    "term",
    "expPrime",
    "factor",
    "termPrime",
    "highPrecedenceOperators",
    "lowPrecedenceOperators",
    "logicalOp",
    "relationalOp",
    "optionalReturn",
    "more_ids",
    "A"
};

// Global FIRST structure: each row corresponds to a nonterminal and each column to a terminal.
typedef struct {
    int table[NUM_NON_TERMINALS][NUM_TERMINALS];
} FirstSets;

FirstSets firstSets;

// Helper: returns the index of a nonterminal in nonterminals[]
int getNonterminalIndex(const char* nt) {
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        if (strcmp(nonterminals[i], nt) == 0)
            return i;
    }
    return -1;
}

// Helper: returns the index of a terminal in terminals[]
int getTerminalIndex(const char* t) {
    for (int i = 0; i < NUM_TERMINALS; i++) {
        if (strcmp(terminals[i], t) == 0)
            return i;
    }
    return -1;
}

// Reads the grammar from a file and stores productions.
void readGrammar(char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(1);
    }
    
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        // Remove newline and skip empty lines.
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0)
            continue;
        
        // Tokenize the line using strtok_r.
        char* tokens[100];
        int tokenCount = 0;
        char *saveptr;
        char *tok = strtok_r(line, " \t", &saveptr);
        while (tok && tokenCount < 100) {
            tokens[tokenCount++] = strdup(tok);
            tok = strtok_r(NULL, " \t", &saveptr);
        }
        
        // Must have at least one LHS and one RHS symbol.
        if (tokenCount < 2) {
            for (int i = 0; i < tokenCount; i++)
                free(tokens[i]);
            continue;
        }
        
        if (prodCount >= MAX_PRODUCTIONS) {
            fprintf(stderr, "Exceeded maximum productions.\n");
            break;
        }
        
        // Create a production: first token is LHS; remaining tokens form the RHS.
        productions[prodCount].lhs = tokens[0]; // Keep tokens[0] as LHS.
        productions[prodCount].rhsCount = tokenCount - 1;
        productions[prodCount].rhs = malloc(sizeof(char*) * (tokenCount - 1));
        for (int i = 1; i < tokenCount; i++) {
            productions[prodCount].rhs[i-1] = tokens[i];
        }
        prodCount++;
    }
    fclose(fp);
}

// Computes the FIRST sets with full propagation (handling nonterminals on the RHS).
void computeFirstSets() {
    // Initialize FIRST table to 0.
    for (int i = 0; i < NUM_NON_TERMINALS; i++)
        for (int j = 0; j < NUM_TERMINALS; j++)
            firstSets.table[i][j] = 0;
    
    int changed = 1;
    while (changed) {
        changed = 0;
        // Process each production A -> X1 X2 ... Xn
        for (int p = 0; p < prodCount; p++) {
            int lhsIndex = getNonterminalIndex(productions[p].lhs);
            if (lhsIndex == -1)
                continue;
            int canDeriveEps = 1;
            for (int i = 0; i < productions[p].rhsCount && canDeriveEps; i++) {
                char *sym = productions[p].rhs[i];
                // If symbol is a terminal
                if (strncmp(sym, "TK_", 3) == 0) {
                    int tIndex = getTerminalIndex(sym);
                    if (tIndex != -1 && !firstSets.table[lhsIndex][tIndex]) {
                        firstSets.table[lhsIndex][tIndex] = 1;
                        changed = 1;
                    }
                    // If the terminal is TK_EPS, we continue; otherwise, break.
                    if (strcmp(sym, "TK_EPS") == 0)
                        canDeriveEps = 1;
                    else
                        canDeriveEps = 0;
                } else {
                    // Symbol is a nonterminal.
                    int ntIndex = getNonterminalIndex(sym);
                    if (ntIndex == -1)
                        continue;
                    // Add FIRST(nonterminal) except TK_EPS to FIRST(lhs).
                    for (int t = 0; t < NUM_TERMINALS; t++) {
                        // Skip TK_EPS (if present, will be handled later)
                        if (strcmp(terminals[t], "TK_EPS") == 0)
                            continue;
                        if (firstSets.table[ntIndex][t] && !firstSets.table[lhsIndex][t]) {
                            firstSets.table[lhsIndex][t] = 1;
                            changed = 1;
                        }
                    }
                    // If FIRST(nonterminal) contains TK_EPS then continue; otherwise stop.
                    int epsIndex = getTerminalIndex("TK_EPS");
                    if (epsIndex != -1 && firstSets.table[ntIndex][epsIndex])
                        canDeriveEps = 1;
                    else
                        canDeriveEps = 0;
                }
            }
            // If all symbols in RHS can derive ε, then add TK_EPS to FIRST(lhs).
            if (canDeriveEps) {
                int epsIndex = getTerminalIndex("TK_EPS");
                if (epsIndex != -1 && !firstSets.table[lhsIndex][epsIndex]) {
                    firstSets.table[lhsIndex][epsIndex] = 1;
                    changed = 1;
                }
            }
        }
    }
}

// Prints the FIRST set for each nonterminal.
void printFirstSets() {
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        printf("FIRST(%s) = { ", nonterminals[i]);
        int printed = 0;
        for (int j = 0; j < NUM_TERMINALS; j++) {
            if (firstSets.table[i][j]) {
                printf("%s ", terminals[j]);
                printed = 1;
            }
        }
        if (!printed)
            printf("∅");
        printf("}\n");
    }
}

// --- FOLLOW Set Structure and Functions ---

// Global FOLLOW structure: each row corresponds to a nonterminal and each column to a terminal.
typedef struct {
    int table[NUM_NON_TERMINALS][NUM_TERMINALS];
} FollowSets;

FollowSets followSets;

// Computes the FOLLOW sets for all nonterminals using the productions and the FIRST sets.
void computeFollowSets() {
    // Initialize the follow table to 0.
    for (int i = 0; i < NUM_NON_TERMINALS; i++)
        for (int j = 0; j < NUM_TERMINALS; j++)
            followSets.table[i][j] = 0;
    
    // For the start symbol (assumed to be nonterminals[0]), add the end-marker (TK_DOLLAR).
    int startIndex = getNonterminalIndex(nonterminals[0]);
    int dollarIndex = getTerminalIndex("TK_DOLLAR");
    if (startIndex != -1 && dollarIndex != -1) {
        followSets.table[startIndex][dollarIndex] = 1;
    }
    
    int changed = 1;
    while (changed) {
        changed = 0;
        // For each production A -> X1 X2 ... Xn
        for (int p = 0; p < prodCount; p++) {
            int A_index = getNonterminalIndex(productions[p].lhs);
            // Process each symbol in the RHS.
            for (int i = 0; i < productions[p].rhsCount; i++) {
                char *sym = productions[p].rhs[i];
                // Only process if sym is a nonterminal (i.e. does not start with "TK_").
                if (strncmp(sym, "TK_", 3) == 0)
                    continue;
                int B_index = getNonterminalIndex(sym);
                if (B_index == -1)
                    continue;
                // Compute FIRST(beta) where beta = X(i+1)...Xn.
                int canDeriveEps = 1;
                // For each symbol in beta:
                for (int j = i + 1; j < productions[p].rhsCount && canDeriveEps; j++) {
                    char *beta_sym = productions[p].rhs[j];
                    // If beta_sym is terminal:
                    if (strncmp(beta_sym, "TK_", 3) == 0) {
                        int tIndex = getTerminalIndex(beta_sym);
                        if (tIndex != -1) {
                            // Add beta_sym to FOLLOW(B) if not TK_EPS.
                            if (strcmp(beta_sym, "TK_EPS") != 0 && !followSets.table[B_index][tIndex]) {
                                followSets.table[B_index][tIndex] = 1;
                                changed = 1;
                            }
                        }
                        // Only continue if beta_sym is TK_EPS.
                        if (strcmp(beta_sym, "TK_EPS") == 0)
                            canDeriveEps = 1;
                        else
                            canDeriveEps = 0;
                    }
                    else { // beta_sym is a nonterminal.
                        int beta_nt = getNonterminalIndex(beta_sym);
                        if (beta_nt == -1)
                            continue;
                        // For each terminal in FIRST(beta_sym) except TK_EPS, add it to FOLLOW(B).
                        for (int t = 0; t < NUM_TERMINALS; t++) {
                            // Skip TK_EPS.
                            if (strcmp(terminals[t], "TK_EPS") == 0)
                                continue;
                            if (firstSets.table[beta_nt][t] && !followSets.table[B_index][t]) {
                                followSets.table[B_index][t] = 1;
                                changed = 1;
                            }
                        }
                        // Continue if FIRST(beta_sym) contains TK_EPS.
                        int epsIndex = getTerminalIndex("TK_EPS");
                        if (epsIndex != -1 && firstSets.table[beta_nt][epsIndex])
                            canDeriveEps = 1;
                        else
                            canDeriveEps = 0;
                    }
                }
                // If beta is empty or all symbols in beta can derive ε,
                // then add FOLLOW(A) to FOLLOW(B).
                if (canDeriveEps) {
                    for (int t = 0; t < NUM_TERMINALS; t++) {
                        if (followSets.table[A_index][t] && !followSets.table[B_index][t]) {
                            followSets.table[B_index][t] = 1;
                            changed = 1;
                        }
                    }
                }
            }
        }
    }
}

// Prints the FOLLOW set for each nonterminal.
void printFollowSets() {
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        printf("FOLLOW(%s) = { ", nonterminals[i]);
        int printed = 0;
        for (int j = 0; j < NUM_TERMINALS; j++) {
            if (followSets.table[i][j]) {
                printf("%s ", terminals[j]);
                printed = 1;
            }
        }
        if (!printed)
            printf("∅");
        printf("}\n");
    }
}

// --- PARSE TABLE STRUCTURE AND FUNCTIONS ---

// Global parse table: rows correspond to nonterminals and columns to terminals.
// Each cell holds the index of the production to use, or -1 if none.
int parseTable[NUM_NON_TERMINALS][NUM_TERMINALS];

// Computes the parse table using the productions along with FIRST and FOLLOW sets.
void computeParseTable() {
    // Initialize parse table cells to -1 (meaning no production).
    for (int i = 0; i < NUM_NON_TERMINALS; i++)
        for (int j = 0; j < NUM_TERMINALS; j++)
            parseTable[i][j] = -1;
    
    // For each production A -> α, determine FIRST(α) and then fill the table.
    for (int p = 0; p < prodCount; p++) {
        int A_index = getNonterminalIndex(productions[p].lhs);
        if (A_index == -1)
            continue;
        
        // Compute FIRST(α) for production p.
        // We'll compute it into an array firstAlpha where firstAlpha[t]==1 means terminals[t] is in FIRST(α)
        int firstAlpha[NUM_TERMINALS] = {0};
        int canDeriveEps = 1; // true if all symbols seen so far can derive ε.
        for (int i = 0; i < productions[p].rhsCount && canDeriveEps; i++) {
            char *sym = productions[p].rhs[i];
            if (strncmp(sym, "TK_", 3) == 0) { // sym is terminal
                int tIndex = getTerminalIndex(sym);
                if (tIndex != -1)
                    firstAlpha[tIndex] = 1;
                // Only continue if the terminal is TK_EPS.
                if (strcmp(sym, "TK_EPS") == 0)
                    canDeriveEps = 1;
                else
                    canDeriveEps = 0;
            } else {
                // sym is a nonterminal; add FIRST(sym) except TK_EPS.
                int ntIndex = getNonterminalIndex(sym);
                if (ntIndex != -1) {
                    for (int t = 0; t < NUM_TERMINALS; t++) {
                        // Skip TK_EPS (if present)
                        if (strcmp(terminals[t], "TK_EPS") == 0)
                            continue;
                        if (firstSets.table[ntIndex][t])
                            firstAlpha[t] = 1;
                    }
                    // Continue if FIRST(sym) contains TK_EPS.
                    int epsIndex = getTerminalIndex("TK_EPS");
                    if (epsIndex != -1 && firstSets.table[ntIndex][epsIndex])
                        canDeriveEps = 1;
                    else
                        canDeriveEps = 0;
                }
            }
        }
        
        // For every terminal a in FIRST(α) (except TK_EPS), add production p to table[A_index][a].
        for (int t = 0; t < NUM_TERMINALS; t++) {
            if (firstAlpha[t]) {
                if (parseTable[A_index][t] != -1) {
                    printf("Conflict in parse table for nonterminal %s, terminal %s\n", nonterminals[A_index], terminals[t]);
                }
                parseTable[A_index][t] = p;
            }
        }
        
        // If α can derive ε (i.e. canDeriveEps is true), then for each terminal b in FOLLOW(A), add production p.
        if (canDeriveEps) {
            for (int t = 0; t < NUM_TERMINALS; t++) {
                if (followSets.table[A_index][t]) {
                    if (parseTable[A_index][t] != -1) {
                        printf("Conflict in parse table for nonterminal %s, terminal %s\n", nonterminals[A_index], terminals[t]);
                    }
                    parseTable[A_index][t] = p;
                }
            }
        }
    }
}

// Prints the parse table.
void printParseTable() {
    printf("Parse Table:\n");
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        printf("%s: ", nonterminals[i]);
        for (int j = 0; j < NUM_TERMINALS; j++) {
            if (parseTable[i][j] != -1) {
                // Print the terminal and the production index.
                printf("[%s: p%d] ", terminals[j], parseTable[i][j]);
            }
        }
        printf("\n");
    }
}



int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Usage: %s grammar.txt\n", argv[0]);
        return 1;
    }
    
    readGrammar(argv[1]);
    computeFirstSets();
	computeFollowSets();
	computeParseTable();
	//printParseTable();
	//printFollowSets();
    //printFirstSets();
	
    
    return 0;
}
