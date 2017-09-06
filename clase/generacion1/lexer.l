%option noyywrap
%option yylineno

%{
    #include <stdlib.h>
    #include <stdio.h>
    #include "tokens.h"

    using namespace std;
%}

%%

\n { return TK_EOL; }
[ \t] {}
[0-9]+ { yylval.int_t = atoi(yytext); return TK_NUM; }
[_a-zA-Z][a-zA-Z0-9_]* { yylval.string_t = new string(yytext); return TK_ID; }
"(" { return '('; }
")" { return ')'; }
"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
.  { return TK_ERR; }

%%