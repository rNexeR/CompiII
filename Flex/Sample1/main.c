#include <stdio.h>

int expr();
int yylex();
extern int token;
extern void stmnts();

int main(){
    token = yylex();
    stmnts();  
}