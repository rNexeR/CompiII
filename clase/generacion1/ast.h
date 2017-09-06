#ifndef _ast_h
#define _ast_h

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <stack>
#include <list>

using namespace std;

class Expr;
class RetData;

enum ExprKind
{
    ADDEXPR,
    SUBEXPR,
    MULEXPR,
    NUMEXPR,
    IDEXPR
};

class RetData
{
  public:
    RetData() {}
    RetData(string code, string place)
    {
        this->code = code;
        this->place = place;
    }
    string code, place;
};

class Expr
{
  public:
    Expr() {}
    virtual RetData *generate_code(stack<string> *_temps) = 0;
    ExprKind kind;
};

class Ast
{
  public:
    Ast()
    {
        this->_temps = new stack<string>();
        for (int i = 0; i < 10; i++)
        {
            char t[4];
            sprintf(t, "$t%d", 9 - i);
            _temps->push(string(t));
        }
    }

    void add_expr(Expr *expr)
    {
        this->exprs.push_back(expr);
    }

    void generate_code()
    {
        list<Expr *>::iterator i;
        for (i = exprs.begin(); i != exprs.end(); i++)
        {
            Expr *t = *i;
            RetData *result = t->generate_code(_temps);
            // printf("[%s] %s\n\n", result->place.c_str(), result->code.c_str());
            printf("%s\n", result->code.c_str());
            delete result;
        }
    }

    list<Expr *> exprs;
    stack<string> *_temps;
};

class NumExpr : public Expr
{
  public:
    NumExpr(int val) : Expr()
    {
        this->value = val;
        this->kind = NUMEXPR;
    }

    RetData *generate_code(stack<string> *_temps)
    {
        RetData *ret = new RetData();
        string code = "";

        string place = _temps->top();
        _temps->pop();
        char val[25];
        sprintf(val, "%d", this->value);
        code = "li " + place + ", " + string(val) + "\n";

        ret->code = code;
        ret->place = place;

        return ret;
    }

    int value;
};

class BinaryExpr : public Expr
{
  public:
    BinaryExpr(Expr *expr1, Expr *expr2) : Expr()
    {
        this->expr1 = expr1;
        this->expr2 = expr2;
    }
    Expr *expr1, *expr2;
    // RetData *generate_code(stack<string>* _temps) { return NULL; }
};

class AddExpr : public BinaryExpr
{
  public:
    AddExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2)
    {
        this->kind = ADDEXPR;
    }

    RetData *generate_code(stack<string> *_temps)
    {
        if (expr1->kind == expr2->kind && expr1->kind == NUMEXPR)
        {
            RetData *ret = new RetData();
            string code = "";

            string place = _temps->top();
            _temps->pop();
            char val[25];
            int result = ((NumExpr*)expr1)->value + ((NumExpr*)expr2)->value;
            sprintf(val, "%d", result);
            code = "li " + place + ", " + string(val) + "\n";

            ret->code = code;
            ret->place = place;

            return ret;
        }

        RetData *ret = new RetData(), *e1, *e2;
        string code = "";

        e1 = this->expr1->generate_code(_temps);
        e2 = this->expr2->generate_code(_temps);

        _temps->push(e2->place);
        _temps->push(e1->place);

        string place = _temps->top();
        _temps->pop();

        code = e1->code + e2->code;

        code += "add " + place + ", " + e1->place + ", " + e2->place + " \n";

        ret->place = place;
        ret->code = code;

        delete e1;
        delete e2;

        return ret;
    }
};

class SubExpr : public BinaryExpr
{
  public:
    SubExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2)
    {
        this->kind = SUBEXPR;
    }

    RetData *generate_code(stack<string> *_temps)
    {
        if (expr1->kind == expr2->kind && expr1->kind == NUMEXPR)
        {
            RetData *ret = new RetData();
            string code = "";

            string place = _temps->top();
            _temps->pop();
            char val[25];
            int result = ((NumExpr*)expr1)->value - ((NumExpr*)expr2)->value;
            sprintf(val, "%d", result);
            code = "li " + place + ", " + string(val) + "\n";

            ret->code = code;
            ret->place = place;

            return ret;
        }
        RetData *ret = new RetData(), *e1, *e2;
        string code = "";

        e1 = this->expr1->generate_code(_temps);
        e2 = this->expr2->generate_code(_temps);

        _temps->push(e2->place);
        _temps->push(e1->place);

        string place = _temps->top();
        _temps->pop();

        code = e1->code + e2->code;

        code += "sub " + place + ", " + e1->place + ", " + e2->place + " \n";

        ret->place = place;
        ret->code = code;

        delete e1;
        delete e2;

        return ret;
    }
};

class MulExpr : public BinaryExpr
{
  public:
    MulExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2)
    {
        this->kind = MULEXPR;
    }

    RetData *generate_code(stack<string> *_temps)
    {
        if (expr1->kind == expr2->kind && expr1->kind == NUMEXPR)
        {
            RetData *ret = new RetData();
            string code = "";

            string place = _temps->top();
            _temps->pop();
            char val[25];
            int result = ((NumExpr*)expr1)->value * ((NumExpr*)expr2)->value;
            sprintf(val, "%d", result);
            code = "li " + place + ", " + string(val) + "\n";

            ret->code = code;
            ret->place = place;

            return ret;
        }
        RetData *ret = new RetData(), *e1, *e2;
        string code = "";

        e1 = this->expr1->generate_code(_temps);
        e2 = this->expr2->generate_code(_temps);

        _temps->push(e2->place);
        _temps->push(e1->place);

        string place = _temps->top();
        _temps->pop();

        code = e1->code + e2->code;

        code += "mul " + e1->place + ", " + e2->place + " \n";
        code += "lflo " + place + "\n";

        ret->place = place;
        ret->code = code;

        delete e1;
        delete e2;

        return ret;
    }
};

class IdExpr : public Expr
{
  public:
    IdExpr(string id) : Expr()
    {
        this->id = id;
        this->kind = IDEXPR;
    }

    RetData *generate_code(stack<string> *_temps)
    {
        RetData *ret = new RetData();
        string code = "";

        string place = _temps->top();
        _temps->pop();
        code = "lw " + place + ", " + this->id + "\n";

        ret->code = code;
        ret->place = place;

        return ret;
    }

    string id;
};
#endif