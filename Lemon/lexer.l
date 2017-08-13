%option noyywrap
%option yylineno
%x comment
%x comment_in_eol
%x include
%x include2
%{
    #include <stdio.h>
    #include "ast.h"
    #include "tokens.h"
    // #include "lexglobal.h"
    #include <iostream>

    using namespace std;

    typedef struct include_stack{
        YY_BUFFER_STATE bs;
        struct include_stack* prev;
        FILE* f;
        int lineno;
        char* filename;
    } include_stack_t;

    include_stack_t* inc_stat = NULL;

    static int input();

    char* yy_filename;
    int push_file(char*);
    int pop_file();
    Token_t *current_token = (Token_t *)(malloc(sizeof(Token_t)));
%}

%%
[ \t]                           { }
"+"                             { return OP_ADD; }
"-"                             { return OP_SUB; }
"*"                             { return OP_MULT; }
"/*"                            { BEGIN(comment); }
"//".*\n                        {  }
"//".*                          { /*Nothing*/ }
"/"                             { return OP_DIV; }
"("                             { return TK_L_PAR; }
")"                             { return TK_R_PAR; }
","                             { return TK_COMMA; }
"DEC"                           { return TK_DEC; }
"HEX"                           { return TK_HEX; }
"BIN"                           { return TK_BIN; }
[0-9]+                          { current_token->int_value = atoi(yytext); return TK_NUMBER; }
"print"                         { return TK_RW_PRINT; }
"if"                            { return TK_RW_IF; }
"="                             { return TK_EQUAL; }
"=="                            { return TK_COMP_EQUAL; }
"!="                            { return TK_COMP_NEQUAL; }
">"                             { return TK_COMP_GREATER; }
">="                            { return TK_COMP_GREATER_EQUAL; }
"<"                             { return TK_COMP_LESS; }
"<="                            { return TK_COMP_LESS_EQUAL; }
"{"                             { return TK_L_BRACE; }
"}"                             { return TK_R_BRACE; }
"else"                          { return TK_RW_ELSE; }
^"#"[ \t]*"include"             { BEGIN(include); }
[_a-zA-Z][a-zA-Z0-9_]*          { current_token->str_value = new string(yytext); return TK_VARIABLE; }
"\n"                            { return TK_EOL; }
<INITIAL><<EOF>>                { if(pop_file() != 0) yyterminate(); return TK_EOL; }
.                               { printf("Line %d: Invalid Token\n", yylineno); }

<comment>"*/"                   { BEGIN(INITIAL); }
<comment>.|\n                   { /* Nothing */ }
<comment><<EOF>>                { printf("Line %d: Block comment without end\n", yylineno); return 0; }

<include>[ \t]*                 { /* Nothing */ }
<include>\".*\"               {
    yytext++;
    yytext[strlen(yytext)-1] = '\0';

    int c;
    while ((c = yyinput()) == ' ' || c == '\t');
    if(c != '\n'){
        printf("Line %d: Bad include expression line\n", yylineno);
        return 0;
    }
    yylineno++;

    if(push_file(yytext) != 0)
        yyterminate();
    BEGIN(INITIAL);
    }
<include>"<".*">"            {
    yytext++;
    yytext[strlen(yytext)-1] = '\0';

    int c;
    while ((c = yyinput()) == ' ' || c == '\t');
    if(c != '\n'){
        printf("Line %d: Bad include expression line\n", yylineno);
        return 0;
    }

    if(push_file(yytext) != 0)
        yyterminate();
    BEGIN(INITIAL);
    }

%%

int push_file(char* filename){
    FILE* f = fopen(filename, "r");

    if (f == NULL)
    {
        fprintf(stderr, "Cannot open file %s\n", filename);
        return 1;
    }

    include_stack_t* is = (include_stack_t*)(malloc(sizeof(include_stack_t)));
    if(is == NULL){
        fprintf(stderr, "Out of memory\n");
        return 1;
    }

    is->f = f;
    is->lineno = yylineno;
    is->bs = yy_create_buffer(f, YY_BUF_SIZE);
    is->prev = inc_stat;
    is->filename = yy_filename;

    inc_stat = is;

    yylineno = 1;
    yy_filename = filename;

    yy_switch_to_buffer(is->bs);

    return 0;
}

int pop_file(){
    include_stack_t* prev = inc_stat->prev;
    fclose(inc_stat->f);
    yy_delete_buffer(inc_stat->bs);

    yylineno = inc_stat->lineno;
    yy_filename = inc_stat->filename;

    free(inc_stat);

    if(prev == NULL) return 1;

    yy_switch_to_buffer(prev->bs);
    inc_stat = prev;

    return 0;
}
