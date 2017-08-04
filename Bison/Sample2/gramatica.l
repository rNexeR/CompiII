%option noyywrap
%option yylineno
%x comment
%x include
%x include2
%{
    #include <stdio.h>
    #include "token.h"
    #include <string.h>

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
[ \t]               { }
"+"                 { return OP_ADD; }
"-"                 { return OP_SUB; }
"*"                 { return OP_MULT; }
"/*"                { BEGIN(comment); }
"//".*\n            { return TK_EOL; }
"//".*              { /*Nothing*/ }
"/"                 { return OP_DIV; }
"("                 { return TK_L_PAR; }
")"                 { return TK_R_PAR; }
","                 { return TK_COMMA; }
"DEC"               { return TK_DEC; }
"HEX"               { return TK_HEX; }
"BIN"               { return TK_BIN; }
[0-9]+              { yylval.int_t = atoi(yytext); return TK_NUMBER; }
"$"[0-7]            { yylval.int_t = atoi(yytext+1); return TK_VARIABLE; }
"print"             { return TK_RW_PRINT; }
"="                 { return TK_EQUAL; }
"=="                { return TK_COMP_EQUAL; }
"!="                { return TK_COMP_NEQUAL; }
">"                 { return TK_COMP_GREATER; }
">="                { return TK_COMP_GREATER_EQUAL; }
"<"                 { return TK_COMP_LESS; }
"<="                { return TK_COMP_LESS_EQUAL; }
"{"                 { return TK_L_BRACE; }
"}"                 { return TK_R_BRACE; }
"if"                { return TK_RW_IF; }
"else"              { return TK_RW_ELSE; }
^"#"[ \t]*"include" { BEGIN(include); } 
"\n"                { return TK_EOL; }
<INITIAL><<EOF>>             { if(pop_file() != 0) yyterminate(); return TK_EOL; }
.                   { return TK_ERROR; }

<comment>"*/"       { BEGIN(INITIAL); }
<comment>.|\n       { /* Nothing */ }
<comment><<EOF>>    { printf("Line %d: Block comment without end\n", yylineno); return 0; }

<include>[ \t]*     { /* Nothing */ }
<include>\".*\"\n   { 
    yytext++;
    yytext[strlen(yytext)-2] = '\0'; 

    if(push_file(yytext) != 0)
        yyterminate();
    BEGIN(INITIAL);
    return TK_EOL; 
    }
<include>"<".*">"*\n     { 
    yytext++;
    yytext[strlen(yytext)-2] = '\0'; 

    if(push_file(yytext) != 0)
        yyterminate();
    BEGIN(INITIAL);
    return TK_EOL;
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