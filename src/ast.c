#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static ASTNode *alloc_node(NodeKind k) {
    ASTNode *n = calloc(1, sizeof(ASTNode));
    if (!n) { fprintf(stderr, "Out of memory\n"); exit(1); }
    n->kind = k;
    return n;
}

ASTNode *make_number(int v) {
    ASTNode *n = alloc_node(NODE_NUMBER);
    n->val.ival = v;
    return n;
}

ASTNode *make_ident(const char *name) {
    ASTNode *n = alloc_node(NODE_IDENT);
    n->val.name = strdup(name);
    return n;
}

ASTNode *make_assign(ASTNode *lhs, ASTNode *rhs) {
    ASTNode *n = alloc_node(NODE_ASSIGN);
    n->left  = lhs;
    n->right = rhs;
    return n;
}

ASTNode *make_binop(char op, ASTNode *l, ASTNode *r) {
    ASTNode *n = alloc_node(NODE_BINOP);
    n->val.op = op;
    n->left   = l;
    n->right  = r;
    return n;
}

ASTNode *make_unary(char op, ASTNode *child) {
    ASTNode *n = alloc_node(NODE_UNARY);
    n->val.op = op;
    n->left   = child;
    return n;
}

ASTNode *make_stmtlist(ASTNode *stmt, ASTNode *rest) {
    ASTNode *n = alloc_node(NODE_STMTLIST);
    n->left  = stmt;
    n->right = rest;
    return n;
}

/* Pretty-print the AST */
void print_ast(ASTNode *node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth * 2; i++) putchar(' ');
    switch (node->kind) {
        case NODE_NUMBER:   printf("NUMBER(%d)\n",   node->val.ival); break;
        case NODE_IDENT:    printf("IDENT(%s)\n",    node->val.name); break;
        case NODE_ASSIGN:
            printf("ASSIGN\n");
            print_ast(node->left,  depth + 1);
            print_ast(node->right, depth + 1);
            break;
        case NODE_BINOP:
            printf("BINOP('%c')\n", node->val.op);
            print_ast(node->left,  depth + 1);
            print_ast(node->right, depth + 1);
            break;
        case NODE_UNARY:
            printf("UNARY('%c')\n", node->val.op);
            print_ast(node->left,  depth + 1);
            break;
        case NODE_STMTLIST:
            printf("STMTLIST\n");
            print_ast(node->left,  depth + 1);
            print_ast(node->right, depth);
            break;
    }
}

void free_ast(ASTNode *node) {
    if (!node) return;
    if (node->kind == NODE_IDENT) free(node->val.name);
    free_ast(node->left);
    free_ast(node->right);
    free(node);
}
