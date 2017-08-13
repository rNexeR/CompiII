#include "ast.h"

#ifndef YYSTYPE
typedef union {
    Token_t *t;
} yystype;
#define YYSTYPE yystype
#define YYSTYPE_IS_TRIVIAL 1
#endif
/* extern YYSTYPE yylval; */
YYSTYPE yylval;