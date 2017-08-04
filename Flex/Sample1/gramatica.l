%option noyywrap
%{
    #include "token.h"
%}

%%
[ \t]   { }
"+"     { return OP_ADD; }
"-"     { return OP_SUB; }
"*"     { return OP_MULT; }
"/"     { return OP_DIV; }
"("     { return TK_LEFT_PAR; }
")"     { return TK_RIGHT_PAR; }
[0-9]+  { return TK_NUMBER; }
"\n"    { return TK_EOL; }
<<EOF>> { return TK_EOF; }
.       { return TK_ERROR; }
%%