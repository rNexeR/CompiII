%token_type { Token * }

%include{
    #include "ast.h"
    #include <stdio.h>
    #include <assert.h>
    #include <list>

    int yylex();
    extern int yylineno;
    extern char* yy_filename;
}

%type stmt { Statement * }
%type assign { Statement * } 
%type print { Statement * } 
%type statement_list { Statement * } 
%type block_statement { Statement * } 
%type if_stmt { Statement * }
%type opt_else { Statement * }
%type while_stmt { Statement * }
%type fn_def { Statement * }
%type return_st { Statement * }
%type function_call { Statement * }

%type opt_param_list { list<string>* }
%type param_list { list<string>* }

%type arg_list { list<Expr*>* }
%type opt_args { list<Expr*>* }

%type expr { Expr * } 
%type term { Expr * } 
%type factor { Expr * } 
%type compare_options { Expr * } 
%type conditional_expr { Expr * }

%type print_option { int }

%default_type { int }

%name NxParser

%parse_accept{
    printf("parse complete!\n");
}

%syntax_error{
    printf("Error at: %d\n", yylineno);
}

%start_symbol input

input ::= opt_eol statement_list(S) opt_eol .     { S->exec(); }

opt_eol ::= TK_EOL .
opt_eol ::= .

statement_list(L) ::= statement_list(Ss) TK_EOL stmt(S) .     { L = Ss; ((BlockStatement *)L)->addStatement(S); }
statement_list(L) ::= stmt(S) .                             { L = new BlockStatement; ((BlockStatement *)L)->addStatement(S); }

stmt(L) ::= print(S) .          { L = S; }
stmt(L) ::= assign(S) .         { L = S; }
stmt(L) ::= if_stmt(S) .        { L = S; }
stmt(L) ::= while_stmt(W) .     { L = W; }
stmt(L) ::= fn_def(D) .         { L = D; }
stmt(L) ::= return_st(R) .      { L = R; }
stmt(L) ::= function_call(C) .  { L = C; }

function_call(L) ::= RW_CALL TK_ID(N) TK_L_PAR opt_args(A) TK_R_PAR . { L = new FunctionCallStatement(N->str_value, A); }

opt_args(L) ::= arg_list(P) .   { L = P; }
opt_args(L) ::= .                 { L = new list<Expr*>(); }

arg_list(L) ::= arg_list(P) TK_COMMA expr(E) . { L = P; L->push_back(E); }
arg_list(L) ::= expr(E) .                              { L = new list<Expr*>(); L->push_back(E); }

return_st(R) ::= RW_RETURN expr(E) . { R = new ReturnStatement(E); }

fn_def(L) ::= RW_FN_DEF TK_ID(N) TK_L_PAR opt_param_list(P) TK_R_PAR block_statement(B) . { L = new FunctionDefinitionStatement(N->str_value,P,B); delete N->str_value; }

opt_param_list(L) ::= param_list(P) .   { L = P; }
opt_param_list(L) ::= .                 { L = new list<string>(); }

param_list(L) ::= param_list(P) TK_COMMA TK_ID(N) . { L = P; L->push_back(*(N->str_value)); }
param_list(L) ::= TK_ID(N) .                              { L = new list<string>(); L->push_back(*(N->str_value)); }

if_stmt(L) ::= TK_RW_IF TK_L_PAR conditional_expr(E) TK_R_PAR TK_EOL block_statement(S) opt_else(OE) .  { L = new IfStatement(E, S, OE); }

opt_else(L) ::= TK_RW_ELSE TK_EOL block_statement(OS) .     { L = OS; }
opt_else(L) ::= .                                           { L = NULL; }

while_stmt(W) ::= TK_RW_WHILE TK_L_PAR conditional_expr(E) TK_R_PAR TK_EOL block_statement(S) .     { W = new WhileStatement(E, S); }

conditional_expr(L) ::= expr(E1) compare_options(O) expr(E2) .  { L = O; ((BinaryExpr*)L)->expr1 = E1; ((BinaryExpr*)L)->expr2 = E2; }


block_statement(L) ::= stmt(S) .                                                   { L = S; }
block_statement(L) ::= TK_L_BRACE TK_EOL statement_list(S) TK_EOL TK_R_BRACE . { L = S; }

compare_options(L) ::= TK_COMP_EQUAL .          { L = new EqualRelationalExpr(); }
compare_options(L) ::= TK_COMP_NEQUAL .         { L = new NotEqualRelationalExpr(); }
compare_options(L) ::= TK_COMP_GREATER .        { L = new GreaterThanRelationalExpr(); }
compare_options(L) ::= TK_COMP_GREATER_EQUAL .  { L = new GreaterOrEqualThanRelationalExpr(); }
compare_options(L) ::= TK_COMP_LESS .           { L = new LessThanRelationalExpr(); }
compare_options(L) ::= TK_COMP_LESS_EQUAL .     { L = new LessOrEqualThanRelationalExpr(); }

print(L) ::= TK_RW_PRINT expr(E) . { L = new PrintStatement(E, DEC); }
print(L) ::= TK_RW_PRINT expr(E) TK_COMMA print_option(O) . { L = new PrintStatement(E, O); }

print_option(L) ::= TK_BIN . { L = BIN; }
print_option(L) ::= TK_HEX . { L = HEX; }
print_option(L) ::= TK_DEC . { L = DEC; }

assign(L) ::= TK_ID(V) TK_EQUAL expr(E) . { L = new AssignStatement(E,V->str_value); delete V->str_value; }

expr(L) ::= expr(E) OP_ADD term(T) .    { L = new AddExpr(E,T); }
expr(L) ::= expr(E) OP_SUB term(T) .    { L = new SubExpr(E,T); }
expr(L) ::= term(T) .                   { L = T; }

term(L) ::=   term(T) OP_MULT factor(F) .   { L = new MulExpr(T, F); }
term(L) ::=   term(T) OP_DIV factor(F) .    { L = new DivExpr(T, F); }
term(L) ::=   factor(F) .                   { L = F; }

factor(L) ::= TK_NUMBER(F) .                { L = new NumberExpr(F->int_value); }
factor(L) ::= TK_ID(F) .              { L = new VarExpr(F->str_value); delete F->str_value; }
factor(L) ::= TK_L_PAR expr(E) TK_R_PAR .    { L = E; }
