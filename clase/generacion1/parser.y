%code requires{
    #include "ast.cpp"
}

%{
    #include <stdio.h>
    #include <stdlib.h>

    using namespace std;

    extern int yylineno;
    int yylex();

    void yyerror(const char* msg){
        printf("Line %d: %s\n", yylineno, msg);        
    }

    #define YYERROR_VERBOSE 1
    #define YYDEBUG 1
%}

%union{
    string * string_t;
    int int_t;
    Ast * ast_t;
    Expr *expr_t;
}

%token<int_t> TK_NUM
%token<string_t> TK_ID
%token RW_PRINT RW_IF RW_WHILE RW_ELSE
%token TK_ERR TK_EOL

%type<ast_t> start expr_list
%type<expr_t> expr factor term

%%

start: opt_stmt_list { $$ = $1; $$->generate_code();}
    ;

opt_stmt_list: stmt_list
    |
;

stmt_list: stmt_list stmt
    | stmt
;

stmt: stmt_print ';'
    | stmt_assign ';'
    | stmt_if
    | stmt_while
    ;

stmt_print: RW_PRINT '(' expr ')'
    ;

stmt_assign: TK_ID '=' expr
    ;

stmt_if: RW_IF '(' expr ')' '{' opt_stmt_list '}' RW_ELSE
    ;

stmt: print { $$ = $1; }
    | assign { $$ = $1; }
    | if_stmt { $$ = $1; }
;


if_stmt: TK_RW_IF TK_L_PAR expr TK_R_PAR '{' opt_stmt_list '}' TK_RW_ELSE '{' opt_stmt_list '}' { $$ = new IfStatement($3, $6, $10); }
    | TK_RW_IF TK_L_PAR expr TK_R_PAR '{' opt_stmt_list '}' { $$ = new IfStatement($3, $6, NULL); }
;

expr_list: expr_list TK_EOL expr { $$ = $1; $$->add_expr($3); }
    | expr { $$ = new Ast(); $$->add_expr($1); }
    ;

expr: term { $$ = $1; }
    | expr '+' term { $$ = new AddExpr($1, $3); }
    | expr '-' term { $$ = new SubExpr($1, $3); }
;

term: factor { $$ = $1; }
    | term '*' factor { $$ = new MulExpr($1, $3); }
;

factor: '(' expr ')' { $$ = $2; }
    | TK_NUM { $$ = new NumExpr($1); }
    | TK_ID { $$ = new IdExpr(*$1); delete $1; }