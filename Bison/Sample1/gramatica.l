%option noyywrap
%{
    #include <stdio.h>
    #include "token.h"
%}

%%
[ \t]   { }
"+"     { return OP_ADD; }
"-"     { return OP_SUB; }
"*"     { return OP_MULT; }
"/"     { return OP_DIV; }
"("     { return TK_L_PAR; }
")"     { return TK_R_PAR; }
[0-9]+  { yylval = atoi(yytext); return TK_NUMBER; }
"\n"    { return TK_EOL; }
.       { return TK_ERROR; }
%%