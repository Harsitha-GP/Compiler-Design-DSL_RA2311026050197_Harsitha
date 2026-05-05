#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

/* Returns number of errors found (0 = success) */
int semantic_check(ASTNode *root);

#endif /* SEMANTIC_H */
