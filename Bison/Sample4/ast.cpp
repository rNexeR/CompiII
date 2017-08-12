#include "ast.h"

int vars[8];
map<string, int> variables;

//Expressions
int AddExpr::eval()
{
    return expr1->eval() + expr2->eval();
}

int SubExpr::eval()
{
    return expr1->eval() - expr2->eval();
}

int MulExpr::eval()
{
    return expr1->eval() * expr2->eval();
}

int DivExpr::eval()
{
    return expr1->eval() / expr2->eval();
}

int VarExpr::eval()
{
    if(variables.find(identifier) != variables.end())
        return variables[this->identifier];
    else
        return 0;
}

int EqualRelationalExpr::eval()
{
    return expr1->eval() == expr2->eval();
}

int NotEqualRelationalExpr::eval()
{
    return expr1->eval() != expr2->eval();
}

int GreaterThanRelationalExpr::eval()
{
    return expr1->eval() > expr2->eval();
}

int GreaterOrEqualThanRelationalExpr::eval()
{
    return expr1->eval() >= expr2->eval();
}

int LessThanRelationalExpr::eval()
{
    return expr1->eval() < expr2->eval();
}

int LessOrEqualThanRelationalExpr::eval()
{
    return expr1->eval() <= expr2->eval();
}

//Statements
void PrintStatement::exec()
{
    // printf("%d %d\n", number, option);
    if (format == DEC)
        printf("DEC: %d\n", expr->eval());
    else if (format == HEX)
        printf("HEX: %X\n", expr->eval());
    else
    {
        int number = expr->eval();
        printf("BIN: ");
        for (int j = 31; j >= 0; j--)
        {
            int result;
            result = number & (1 << j);
            if (result)
                printf("1");
            else
                printf("0");
        }
        printf("\n");
    }
}

void AssignStatement::exec()
{
    if(variables.find(identifier) != variables.end())
        variables[identifier] = expr->eval();
    else
        variables.insert( pair<string, int>(identifier, expr->eval()) );
}

void BlockStatement::exec()
{
    for (list<Statement *>::iterator i = statementList.begin(); i != statementList.end(); i++)
    {
        (*i)->exec();
    }
}

void IfStatement::exec()
{
    if(expr->eval()){
        body->exec();
    }else if(else_body != NULL){
        else_body->exec();
    }
}