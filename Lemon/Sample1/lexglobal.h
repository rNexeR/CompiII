#include "ast.h"

#ifndef YYSTYPE
typedef union {
    char *lex;
} yystype;
#define YYSTYPE yystype
#define YYSTYPE_IS_TRIVIAL 1
#endif
/* extern YYSTYPE yylval; */
YYSTYPE yylval;