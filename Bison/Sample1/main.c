#include <stdio.h>

int expr();
int yylex();
void yyparse();

int main(){
    // token = yylex();
    // printf("Resultado: %d \n", expr());  
    yyparse();
}