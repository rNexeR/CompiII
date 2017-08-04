// C-CODE
%{
#include <stdio.h>
int yylex();
void yyerror(const char* msg){
    printf("%s\n",msg);
}
#define YYERROR_VERBOSE 1
%}

// TOKENS
%token OP_ADD OP_SUB OP_MULT OP_DIV TK_L_PAR TK_R_PAR TK_NUMBER 
%token TK_EOL TK_EOF TK_ERROR

%%

//GRAMMAR
start   :   expr { printf("Resultado: %d \n", $1); }
        |   expr TK_EOL start { printf("Resultado: %d \n", $1); }
        |   
; 

expr    :   expr OP_ADD term { $$ = $1 + $3; }
        |   expr OP_SUB term { $$ = $1 - $3; }
        |   term { $$ = $1; }
;

term    :   term OP_MULT factor { $$ = $1 * $3; }
        |   term OP_DIV factor { $$ = $1 / $3; }
        |   factor { $$ = $1; }
;

factor  :   TK_NUMBER { $$ = $1; }
        |   TK_L_PAR expr TK_R_PAR { $$ = $2; } 
;