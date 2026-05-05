#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "codegen.h"

static int   temp_count;
static FILE *out_fp;

static void emit(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(out_fp, fmt, ap);
    va_end(ap);
}

/* Forward declaration */
static char *gen_expr(ASTNode *node);

/* Returns the name that holds the value of `node` (caller must free) */
static char *gen_expr(ASTNode *node) {
    char buf[64];
    if (!node) { return strdup("??"); }

    switch (node->kind) {
        case NODE_NUMBER:
            snprintf(buf, sizeof(buf), "%d", node->val.ival);
            return strdup(buf);

        case NODE_IDENT:
            return strdup(node->val.name);

        case NODE_UNARY: {
            char *operand = gen_expr(node->left);
            snprintf(buf, sizeof(buf), "t%d", temp_count++);
            emit("    %s = -%s\n", buf, operand);
            free(operand);
            return strdup(buf);
        }

        case NODE_BINOP: {
            char *lv = gen_expr(node->left);
            char *rv = gen_expr(node->right);
            snprintf(buf, sizeof(buf), "t%d", temp_count++);
            emit("    %s = %s %c %s\n", buf, lv, node->val.op, rv);
            free(lv); free(rv);
            return strdup(buf);
        }

        default:
            return strdup("??");
    }
}

static void gen_stmt(ASTNode *node) {
    if (!node) return;
    if (node->kind == NODE_STMTLIST) {
        gen_stmt(node->left);
        gen_stmt(node->right);
        return;
    }
    if (node->kind == NODE_ASSIGN) {
        const char *lhs = node->left->val.name;
        /* Check if RHS is a simple literal — emit direct assignment */
        if (node->right->kind == NODE_NUMBER) {
            emit("    %s = %d\n", lhs, node->right->val.ival);
        } else {
            char *tmp = gen_expr(node->right);
            emit("    %s = %s\n", lhs, tmp);
            free(tmp);
        }
    }
}

void codegen(ASTNode *root, FILE *out) {
    out_fp     = out ? out : stdout;
    temp_count = 0;

    emit("; -- Three-Address Code ----------------------\n");
    emit("begin:\n");
    gen_stmt(root);
    emit("end:\n");
    emit("; --------------------------------------------\n");
}
