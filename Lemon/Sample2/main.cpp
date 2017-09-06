#include <stdio.h>
#include <stdlib.h>

using namespace std;

void* ParseAlloc( void* (*allocProc)(size_t) );
void* Parse(void*, int, int);
void* ParseFree(void*, void (*freeProc)(void*));
extern int current_lexema;
int yylex();

int main(){

    void* parser = ParseAlloc(malloc);

    int tk = yylex();

    while(tk != 0){
        Parse(parser, tk, current_lexema);
        tk = yylex();
    }

    Parse(parser, 0, 0);
    ParseFree(parser, free);

    return 0;
}