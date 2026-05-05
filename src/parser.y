%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* Defined in lexer */
extern int  yylineno;
extern int  yylex(void);

/* Root of the AST after parsing */
ASTNode *ast_root = NULL;

void yyerror(const char *msg) {
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, msg);
    exit(1);
}
%}

%union {
    int      ival;
    char    *sval;
    ASTNode *node;
}

%token <ival> NUMBER
%token <sval> IDENTIFIER
%token ASSIGN PLUS MINUS TIMES DIVIDE LPAREN RPAREN SEMI

%type <node> program stmtlist stmt expr

/* Operator precedence – lowest to highest */
%left  PLUS MINUS
%left  TIMES DIVIDE
%right UMINUS

%%

program
    : stmtlist          { ast_root = $1; }
    ;

stmtlist
    : stmt              { $$ = make_stmtlist($1, NULL); }
    | stmt stmtlist     { $$ = make_stmtlist($1, $2);   }
    ;

stmt
    : IDENTIFIER ASSIGN expr SEMI
                        { $$ = make_assign(make_ident($1), $3); free($1); }
    ;

expr
    : NUMBER            { $$ = make_number($1); }
    | IDENTIFIER        { $$ = make_ident($1); free($1); }
    | expr PLUS  expr   { $$ = make_binop('+', $1, $3); }
    | expr MINUS expr   { $$ = make_binop('-', $1, $3); }
    | expr TIMES expr   { $$ = make_binop('*', $1, $3); }
    | expr DIVIDE expr  { $$ = make_binop('/', $1, $3); }
    | MINUS expr %prec UMINUS
                        { $$ = make_unary('-', $2); }
    | LPAREN expr RPAREN { $$ = $2; }
    ;

%%
