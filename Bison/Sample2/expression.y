// C-CODE
%code requires{
        #include "ast.h"
}

%{
#include <stdio.h>
int yylex();
extern int yylineno;
extern char* yy_filename;
void yyerror(const char* msg){
    printf("%s:%d %s\n", yy_filename, yylineno, msg);
}
#define YYERROR_VERBOSE 1
%}

%union {
        Statement *statement_t;
        Expr *expr_t;
        int int_t;
}

%type<statement_t> stmt assign print statement_list block_statement if_stmt optional_else_stmt
%type<expr_t> expr term factor compare_options conditional_expr
%type<int_t> print_option


// TOKENS
%token OP_ADD OP_SUB OP_MULT OP_DIV TK_L_PAR TK_R_PAR 
%token<int_t> TK_NUMBER TK_VARIABLE TK_HEX TK_BIN TK_DEC
%token TK_EOL TK_EOF TK_ERROR TK_EQUAL TK_RW_PRINT TK_COMMA TK_RW_IF 
%token TK_RW_ELSE TK_R_BRACE TK_L_BRACE TK_COMP_GREATER TK_COMP_GREATER_EQUAL
%token  TK_COMP_LESS TK_COMP_NEQUAL TK_COMP_LESS_EQUAL TK_COMP_EQUAL 

%%

//GRAMMAR

input   :       opt_eols statement_list opt_eols        { $2->exec(); }
;

statement_list  :       statement_list eols stmt   { $$ = $1; ((BlockStatement *)$$)->addStatement($3); }
                |       stmt                       { $$ = new BlockStatement; ((BlockStatement *)$$)->addStatement($1); }
;


stmt    :       print       { $$ = $1; }
        |       assign      { $$ = $1; }
        |       if_stmt     { $$ = $1; }
;


opt_eols        :       eols
                |
;

opt_eol         :       TK_EOL
                |       
;

eols    :       eols TK_EOL
        |       TK_EOL
;

if_stmt :       TK_RW_IF TK_L_PAR conditional_expr TK_R_PAR block_statement optional_else_stmt  { $$ = new IfStatement($3, $5, $6); }
;

conditional_expr        :       expr compare_options expr       { $$ = $2; ((BinaryExpr*)$$)->expr1 = $1; ((BinaryExpr*)$$)->expr2 = $3; }
;

optional_else_stmt      :       TK_RW_ELSE block_statement      { $$ = $2; }
                        |                                       { $$ = NULL; }                                              
;

block_statement :       opt_eol stmt opt_eol                               { $$ = $2; }
                |       opt_eol TK_L_BRACE statement_list TK_R_BRACE opt_eol    { $$ = $3; }
;

compare_options :       TK_COMP_EQUAL           { $$ = new EqualRelationalExpr(); }
                |       TK_COMP_NEQUAL          { $$ = new NotEqualRelationalExpr(); }
                |       TK_COMP_GREATER         { $$ = new GreaterThanRelationalExpr(); }
                |       TK_COMP_GREATER_EQUAL   { $$ = new GreaterOrEqualThanRelationalExpr(); }
                |       TK_COMP_LESS            { $$ = new LessThanRelationalExpr(); }
                |       TK_COMP_LESS_EQUAL      { $$ = new LessOrEqualThanRelationalExpr(); }
;

print   :   TK_RW_PRINT expr { $$ = new PrintStatement($2, DEC); }
        |   TK_RW_PRINT expr TK_COMMA print_option { $$ = new PrintStatement($2, $4); }
;

print_option    :       TK_BIN { $$ = BIN; }
                |       TK_HEX { $$ = HEX; }
                |       TK_DEC { $$ = DEC; }

assign  :   TK_VARIABLE TK_EQUAL expr { /*arreglo[$1] = $3;*/ $$ = new AssignStatement($3,$1); }
;

expr    :   expr OP_ADD term { $$ = new AddExpr($1,$3); }
        |   expr OP_SUB term { $$ = new SubExpr($1,$3); }
        |   term { $$ = $1; }
;

term    :   term OP_MULT factor { $$ = new MulExpr($1, $3); }
        |   term OP_DIV factor { $$ = new DivExpr($1, $3); }
        |   factor { $$ = $1; }
;

factor  :   TK_NUMBER { $$ = new NumberExpr($1); }
        |   TK_VARIABLE { $$ = new VarExpr($1); }
        |   TK_L_PAR expr TK_R_PAR { $$ = $2; } 
;