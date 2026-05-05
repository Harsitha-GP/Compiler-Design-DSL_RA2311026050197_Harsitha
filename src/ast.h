#ifndef AST_H
#define AST_H

/* Node kinds */
typedef enum {
    NODE_NUMBER,    /* integer literal          */
    NODE_IDENT,     /* variable reference        */
    NODE_ASSIGN,    /* x = expr                 */
    NODE_BINOP,     /* expr op expr             */
    NODE_UNARY,     /* unary minus              */
    NODE_STMTLIST   /* ordered list of stmts    */
} NodeKind;

typedef struct ASTNode {
    NodeKind kind;
    union {
        int   ival;         /* NODE_NUMBER              */
        char *name;         /* NODE_IDENT               */
        char  op;           /* NODE_BINOP / NODE_UNARY  */
    } val;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

/* Constructors */
ASTNode *make_number(int n);
ASTNode *make_ident(const char *name);
ASTNode *make_assign(ASTNode *lhs, ASTNode *rhs);
ASTNode *make_binop(char op, ASTNode *l, ASTNode *r);
ASTNode *make_unary(char op, ASTNode *child);
ASTNode *make_stmtlist(ASTNode *stmt, ASTNode *rest);

/* Utilities */
void print_ast(ASTNode *node, int depth);
void free_ast(ASTNode *node);

#endif /* AST_H */
