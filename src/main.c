#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantic.h"
#include "codegen.h"

/* Provided by parser.y */
extern ASTNode *ast_root;
extern FILE    *yyin;
extern int      yyparse(void);

/* ── Banner helpers ─────────────────────────────────────────────── */
static void banner(const char *title) {
    printf("+------------------------------------------+\n");
    printf("|  %-40s|\n", title);
    printf("+------------------------------------------+\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source.dsl> [output.tac]\n", argv[0]);
        return 1;
    }

    /* Open source file */
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        fprintf(stderr, "Error: cannot open '%s'\n", argv[1]);
        return 1;
    }

    printf("\nDSL Compiler  -  source: %s\n\n", argv[1]);

    /* ── Phase 1 & 2 : Lexing + Parsing ──────────────────────── */
    banner("Phase 1 & 2 : Lexing / Parsing");
    yyparse();
    fclose(yyin);
    printf("[Parser] AST constructed successfully\n\n");

    /* ── Phase 3 : AST ────────────────────────────────────────── */
    banner("Phase 3 : Abstract Syntax Tree");
    print_ast(ast_root, 0);
    printf("\n");

    /* ── Phase 4 : Semantic Analysis ──────────────────────────── */
    banner("Phase 4 : Semantic Analysis");
    int errs = semantic_check(ast_root);
    if (errs > 0) {
        printf("[Semantic] %d error(s) found\n", errs);
        printf("(compilation aborted - code generation skipped)\n\n");
        free_ast(ast_root);
        return 1;
    }
    printf("[Semantic] OK - no errors found\n\n");

    /* ── Phase 5 : Code Generation ────────────────────────────── */
    banner("Phase 5 : Intermediate Code Generation");

    FILE *tac_out = NULL;
    if (argc >= 3) {
        tac_out = fopen(argv[2], "w");
        if (!tac_out) {
            fprintf(stderr, "Warning: cannot open output file '%s'\n", argv[2]);
        }
    }

    codegen(ast_root, stdout);          /* always print to stdout */
    if (tac_out) {
        codegen(ast_root, tac_out);     /* also write to file */
        fclose(tac_out);
        printf("[TAC written to: %s]\n", argv[2]);
    }

    printf("\n[Done] Compilation finished successfully.\n\n");

    free_ast(ast_root);
    return 0;
}
