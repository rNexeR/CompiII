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
%token TK_ERR TK_EOL

%type<ast_t> start expr_list
%type<expr_t> expr factor term

%%

start: expr_list { $$ = $1; $$->generate_code();}
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