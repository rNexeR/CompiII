#include <stdio.h>
#include <stdlib.h>
#include "token.h"

int token;

int stmnt();
int expr();
int term();
int factor();
int yylex();
extern char *yytext;

void stmnts()
{
    while (token != TK_EOF)
    {
        printf("Resultado: %d\n", stmnt());
    }
}

int stmnt()
{
    int result = expr();
    if (token == TK_EOL)
        token = yylex();
    else if (token != TK_EOF)
        printf("End of line expected, %s provided\n", yytext);
    return result;
}

int expr()
{
    int result = term();

    switch (token)
    {

    case OP_ADD:
    case OP_SUB:
    {
        while (token == OP_ADD || token == OP_SUB)
        {
            int op = token;
            token = yylex();
            if (op == OP_SUB)
                result -= term();
            else
                result += term();
        }
    }
    break;

    default:
        break;
    }

    return result;
}

int term()
{
    int result = factor();

    switch (token)
    {

    case OP_MULT:
    case OP_DIV:
    {
        while (token == OP_DIV || token == OP_MULT)
        {
            int op = token;
            token = yylex();
            if (op == OP_DIV)
            {
                result /= factor();
            }
            else
            {
                result *= factor();
            }
        }
    }
    break;

    default:
        break;
    }

    return result;
}

int factor()
{
    switch (token)
    {

    case TK_NUMBER:
    {
        int number = atoi(yytext);
        token = yylex();
        return number;
    } //break;

    case TK_LEFT_PAR:
    {
        token = yylex();
        int result = expr();
        if (token == TK_RIGHT_PAR)
            token = yylex();
        else
            printf("Error: ')' expected \n");
        return result;
    } //break;

    default:
    {
        printf("Error: '(' or Number expected \n");
        return 0;
    } //break;
    }
}