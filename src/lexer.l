%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"
%}

%option noyywrap

DIGIT   [0-9]
ALPHA   [a-zA-Z_]
ALNUM   [a-zA-Z0-9_]

%%

[ \t\r]+            { /* skip whitespace */ }
"\n"                { yylineno++; }
"#"[^\n]*           { /* skip comment */ }

{DIGIT}+            { yylval.ival = atoi(yytext); return NUMBER; }
{ALPHA}{ALNUM}*     { yylval.sval = strdup(yytext); return IDENTIFIER; }

"="                 { return ASSIGN; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return TIMES; }
"/"                 { return DIVIDE; }
"("                 { return LPAREN; }
")"                 { return RPAREN; }
";"                 { return SEMI; }

.                   { fprintf(stderr, "Lexer error: unknown character '%s' at line %d\n",
                              yytext, yylineno); exit(1); }

%%
