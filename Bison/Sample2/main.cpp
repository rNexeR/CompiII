#include <stdio.h>

int expr();
int yylex();
void yyparse();
extern FILE *yyin;
int push_file(char*);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input file>[]\n", argv[0]);
        return 1;
    }

    if(push_file(argv[1]) != 0) return 1;
    yyparse();


    // for (int i = 0; i < argc - 1; i++)
    // {

    //     yyin = fopen(argv[1], "r");

    //     if (yyin == NULL)
    //     {
    //         fprintf(stderr, "Cannot open file %s\n", argv[i+1]);
    //         return 1;
    //     }

    //     printf("Executing file %s\n", argv[i+1]);


    //     fclose(yyin);
    // }
}