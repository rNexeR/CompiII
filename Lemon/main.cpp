#include <stdio.h>
#include "ast.h"
#include "tokens.h"

extern FILE *yyin;
int push_file(char *);

void *NxParserAlloc(void *(*allocProc)(size_t));
void *NxParser(void *, int, Token_t *);
void *NxParserFree(void *, void (*freeProc)(void *));
int yylex();
extern Token_t *current_token;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input file>[]\n", argv[0]);
        return 1;
    }

    if (push_file(argv[1]) != 0)
        return 1;

    void *parser = NxParserAlloc(malloc);
    int token = yylex();

    while (token != 0)
    {
        if(token == TK_EOL){
            while( (token = yylex()) == TK_EOL );
            if(token == 0) break;

            if(token == TK_RW_ELSE){
                NxParser(parser, TK_RW_ELSE, 0);
            }else{
                NxParser(parser, TK_EOL, 0);
                NxParser(parser, token, current_token);
            }
        }else{
            NxParser(parser, token, current_token);
        }
        token = yylex();
    }
    printf("Fin\n");
    NxParser(parser, 0, 0);
    printf("/Fin\n");

    NxParserFree(parser, free);
}