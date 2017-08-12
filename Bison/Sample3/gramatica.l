%option noyywrap
%option yylineno
%x comment
%x comment_in_eol
%x include
%x include2
%x end_lines
%{
    #include <stdio.h>
    #include "token.h"
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
%}

%%
[ \t]                           { }
"+"                             { return OP_ADD; }
"-"                             { return OP_SUB; }
"*"                             { return OP_MULT; }
"/*"                            { BEGIN(comment); }
"//".*\n                        { BEGIN(end_lines); }
"//".*                          { /*Nothing*/ }
"/"                             { return OP_DIV; }
"("                             { return TK_L_PAR; }
")"                             { return TK_R_PAR; }
","                             { return TK_COMMA; }
"DEC"                           { return TK_DEC; }
"HEX"                           { return TK_HEX; }
"BIN"                           { return TK_BIN; }
[0-9]+                          { yylval.int_t = atoi(yytext); return TK_NUMBER; }
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
^"#"[ \t]*"include"             { BEGIN(include); }
[_a-zA-Z][a-zA-Z0-9_]*          { string *t = new string(yytext); yylval.string_t = t; return TK_VARIABLE; }
"\n"                            { BEGIN(end_lines); }
<INITIAL><<EOF>>                { if(pop_file() != 0) yyterminate(); return TK_EOL; }
.                               { return TK_ERROR; }

<comment>"*/"                   { BEGIN(INITIAL); }
<comment>.|\n                   { /* Nothing */ }
<comment><<EOF>>                { printf("Line %d: Block comment without end\n", yylineno); return 0; }

<comment_in_eol>"*/"                   { BEGIN(end_lines); }
<comment_in_eol>.|\n                   { /* Nothing */ }
<comment_in_eol><<EOF>>                { printf("Line %d: Block comment without end\n", yylineno); return 0; }

<end_lines>\n                   { }
<end_lines>"//".*\n             { }
<end_lines>"/*"                 { BEGIN(comment_in_eol); }
<end_lines>[ \t]                { }
<end_lines>^"#"[ \t]*"include"  { BEGIN(include); return TK_EOL;}
<end_lines>"else"               { BEGIN(INITIAL); return TK_RW_ELSE; }
<end_lines><<EOF>>              { BEGIN(INITIAL); return TK_EOL; }
<end_lines>.                    { BEGIN(INITIAL); unput(yytext[0]); return TK_EOL; }

<include>[ \t]*                 { /* Nothing */ }
<include>\".*\"\n               {
    yytext++;
    yytext[strlen(yytext)-2] = '\0';

    if(push_file(yytext) != 0)
        yyterminate();
    BEGIN(INITIAL);
    }
<include>"<".*">"*\n            {
    yytext++;
    yytext[strlen(yytext)-2] = '\0';

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
