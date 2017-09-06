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
    virtual bool is_immediate() = 0;
    virtual int get_immediate() = 0;
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
            _temps->push(result->place);
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

    bool is_immediate() { return true; }
    int get_immediate() { return this->value; }

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
    bool is_immediate() { return expr1->is_immediate() && expr2->is_immediate(); }
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
        RetData *ret = new RetData();
        string code = "";
        string place;
        if (this->is_immediate())
        {
            place = _temps->top();
            _temps->pop();
            char val[25];
            int result = get_immediate();
            sprintf(val, "%d", result);
            code = "li " + place + ", " + string(val) + "\n";
        }
        else if (expr1->is_immediate() && expr2->kind == IDEXPR || expr2->is_immediate() && expr1->kind == IDEXPR)
        {
            Expr *imm = expr1->is_immediate() ? expr1 : expr2;
            Expr *id = expr1->kind == IDEXPR ? expr1 : expr2;

            int imm_val = imm->get_immediate();
            RetData *id_ret = id->generate_code(_temps);

            place = id_ret->place;
            char val[25];
            sprintf(val, "%d", imm_val);

            code = id_ret->code;
            code += "addi " + place + ", " + place + ", " + string(val) + "\n";
        }
        else
        {
            RetData *e1, *e2;

            e1 = this->expr1->generate_code(_temps);
            e2 = this->expr2->generate_code(_temps);

            _temps->push(e2->place);
            _temps->push(e1->place);

            place = _temps->top();
            _temps->pop();

            code = e1->code + e2->code;

            code += "add " + place + ", " + e1->place + ", " + e2->place + " \n";

            delete e1;
            delete e2;
        }

        ret->code = code;
        ret->place = place;

        return ret;
    }

    int get_immediate()
    {
        return expr1->get_immediate() + expr2->get_immediate();
    }
};

class SubExpr : public BinaryExpr
{
  public:
    SubExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2)
    {
        this->kind = SUBEXPR;
    }

    int get_immediate()
    {
        return expr1->get_immediate() - expr2->get_immediate();
    }

    RetData *generate_code(stack<string> *_temps)
    {
        RetData *ret = new RetData();
        string code = "";
        string place;
        if (this->is_immediate())
        {
            place = _temps->top();
            _temps->pop();
            char val[25];
            int result = this->get_immediate();
            sprintf(val, "%d", result);
            code = "li " + place + ", " + string(val) + "\n";
        }
        else if (expr2->is_immediate() && expr1->kind == IDEXPR)
        {
            Expr *imm = expr2;
            Expr *id =  expr1;

            int imm_val = imm->get_immediate();
            RetData *id_ret = id->generate_code(_temps);

            place = id_ret->place;
            char val[25];
            sprintf(val, "-%d", imm_val);

            code = id_ret->code;
            code += "addi " + place + ", " + place + ", " + string(val) + "\n";
        }
        else
        {
            RetData *e1, *e2;
            e1 = this->expr1->generate_code(_temps);
            e2 = this->expr2->generate_code(_temps);

            _temps->push(e2->place);
            _temps->push(e1->place);

            place = _temps->top();
            _temps->pop();

            code = e1->code + e2->code;

            code += "sub " + place + ", " + e1->place + ", " + e2->place + " \n";

            delete e1;
            delete e2;
        }

        ret->place = place;
        ret->code = code;

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

    int get_immediate()
    {
        return expr1->get_immediate() * expr2->get_immediate();
    }

    RetData *generate_code(stack<string> *_temps)
    {
        if (this->is_immediate())
        {
            RetData *ret = new RetData();
            string code = "";

            string place = _temps->top();
            _temps->pop();
            char val[25];
            int result = get_immediate();
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

    bool is_immediate() { return false; }
    int get_immediate() { return -1; }

    string id;
};
#endif