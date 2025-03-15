#ifndef PARSERDEF
#define PARSERDEF
#define MAX_CHILDREN 1000
#define MAX_TOKEN_LENGTH 100
// Define your structures here (or in separate header files)
// Example for parse tree node:
struct parseTreeNode {
    tokenInfo token;  // valid only for leaf nodes
    char nonTerminal[50];  // for non-leaf nodes
    int lineNo;
    struct parseTreeNode *parent;
    struct parseTreeNode *children[MAX_CHILDREN]; // Define MAX_CHILDREN
    int numChildren;
    int isLeaf; // 1 for yes, 0 for no
};

typedef struct parseTreeNode parseTreeNode;
typedef parseTreeNode* ParseTree;

// Structure for a single production rule
typedef struct {
    char lhs[MAX_TOKEN_LENGTH]; // Left-hand side symbol
    char **rhs;                 // Array of right-hand side symbols (strings)
    int rhsCount;               // Number of symbols in RHS
} Production;

// Structure for the complete grammar
typedef struct {
    Production *productions;    // Array of production rules
    int productionCount;        // Number of production rules
} Grammar;

#endif // !PARSERDEF
