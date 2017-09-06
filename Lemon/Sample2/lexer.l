%option noyywrap
%{
    #include <stdio.h>
    #include "tokens.h"

    int current_lexema;
%}

%%
[ \t]   { }
"+"     { return OP_ADD; }
"-"     { return OP_SUB; }
"*"     { return OP_MULT; }
"/"     { return OP_DIV; }
"("     { return TK_L_PAR; }
")"     { return TK_R_PAR; }
[0-9]+  { current_lexema = atoi(yytext); return TK_NUMBER; }

%%