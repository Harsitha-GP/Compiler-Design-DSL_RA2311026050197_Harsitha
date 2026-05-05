#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

/* ── Simple open-addressing hash map ─────────────────────────── */
#define HTABLE_SIZE 256

typedef struct Entry {
    char *key;
    struct Entry *next;
} Entry;

static Entry *htable[HTABLE_SIZE];
static int    error_count;

static unsigned int hash(const char *s) {
    unsigned int h = 5381;
    while (*s) h = h * 33 ^ (unsigned char)*s++;
    return h % HTABLE_SIZE;
}

static int sym_defined(const char *name) {
    unsigned int h = hash(name);
    for (Entry *e = htable[h]; e; e = e->next)
        if (strcmp(e->key, name) == 0) return 1;
    return 0;
}

static void sym_define(const char *name) {
    unsigned int h = hash(name);
    Entry *e = malloc(sizeof(Entry));
    e->key  = strdup(name);
    e->next = htable[h];
    htable[h] = e;
}

static void sym_reset(void) {
    for (int i = 0; i < HTABLE_SIZE; i++) {
        Entry *e = htable[i];
        while (e) {
            Entry *nx = e->next;
            free(e->key); free(e);
            e = nx;
        }
        htable[i] = NULL;
    }
}

/* ── Recursive AST walk ───────────────────────────────────────── */
static void check_expr(ASTNode *node) {
    if (!node) return;
    switch (node->kind) {
        case NODE_NUMBER: break;
        case NODE_IDENT:
            if (!sym_defined(node->val.name)) {
                fprintf(stderr, "Semantic error: variable '%s' used before assignment\n",
                        node->val.name);
                error_count++;
            }
            break;
        case NODE_BINOP:
            check_expr(node->left);
            check_expr(node->right);
            break;
        case NODE_UNARY:
            check_expr(node->left);
            break;
        default: break;
    }
}

static void check_stmt(ASTNode *node) {
    if (!node) return;
    if (node->kind == NODE_STMTLIST) {
        check_stmt(node->left);
        check_stmt(node->right);
        return;
    }
    if (node->kind == NODE_ASSIGN) {
        check_expr(node->right);            /* check RHS first */
        sym_define(node->left->val.name);   /* then declare LHS */
    }
}

/* ── Public interface ─────────────────────────────────────────── */
int semantic_check(ASTNode *root) {
    sym_reset();
    error_count = 0;
    check_stmt(root);
    return error_count;
}
